/********************************************************
 * Description : process class in unix
 * Date        : 2016-09-09 15:20:14
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef _MSC_VER


#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include "process/unix_process.h"
#include "string/string.h"
#include "utility/guard.h"

#ifdef _MAC_OS
    #include <sys/event.h>
#else
    #include <sys/inotify.h>
#endif // _MAC_OS

struct process_info_t
{
    unsigned int pid;
    unsigned int ppid;
};

static void get_all_process(std::list<process_info_t> & process_info_list)
{
    process_info_list.clear();

    const char * command = "ps -eo pid,ppid";
    FILE * file = popen(command, "r");
    if (nullptr == file)
    {
        return;
    }

    const std::string spaces(g_blank_character_set);
    const size_t buff_size = 2048;
    char buff[buff_size] = { 0 };
    while (nullptr != fgets(buff, buff_size, file))
    {
        std::string line(buff);

        process_info_t process_info;

        std::string::size_type pid_b = line.find_first_not_of(spaces, 0);
        if (std::string::npos == pid_b)
        {
            continue;
        }
        std::string::size_type pid_e = line.find_first_of(spaces, pid_b);
        if (std::string::npos == pid_e)
        {
            continue;
        }
        std::string pid_value(line.begin() + pid_b, line.begin() + pid_e);
        if (!Goofer::goofer_string_to_type(pid_value, process_info.pid))
        {
            continue;
        }

        std::string::size_type ppid_b = line.find_first_not_of(spaces, pid_e);
        if (std::string::npos == ppid_b)
        {
            continue;
        }
        std::string::size_type ppid_e = line.find_first_of(spaces, ppid_b);
        if (std::string::npos == ppid_e)
        {
            ppid_e = line.size();
        }
        std::string ppid_value(line.begin() + ppid_b, line.begin() + ppid_e);
        if (!Goofer::goofer_string_to_type(ppid_value, process_info.ppid))
        {
            continue;
        }

        process_info_list.push_back(process_info);
    }

    pclose(file);
}

static bool get_process_tree(unsigned int process_id, std::list<unsigned int> & process_id_tree)
{
    process_id_tree.clear();

    if (0 == process_id)
    {
        return (false);
    }

    std::list<process_info_t> process_info_list;
    get_all_process(process_info_list);

    process_id_tree.push_back(process_id);

    while (true)
    {
        bool find_child_process = false;

        std::list<process_info_t>::iterator iter = process_info_list.begin();
        while (process_info_list.end() != iter)
        {
            if (process_id_tree.end() != std::find(process_id_tree.begin(), process_id_tree.end(), iter->ppid))
            {
                process_id_tree.push_back(iter->pid);
                iter = process_info_list.erase(iter);
                find_child_process = true;
            }
            else
            {
                ++iter;
            }
        }

        if (!find_child_process)
        {
            break;
        }
    }

    return (true);
}

static void kill_process(unsigned int process_id, int exit_code)
{
    if (0 == process_id)
    {
        return;
    }

    kill(static_cast<pid_t>(process_id), exit_code);
}

static void kill_process_tree(unsigned int process_id, int exit_code)
{
    if (0 == process_id)
    {
        return;
    }

    std::list<unsigned int> process_id_tree;
    get_process_tree(process_id, process_id_tree);

    for (std::list<unsigned int>::reverse_iterator iter = process_id_tree.rbegin(); process_id_tree.rend() != iter; ++iter)
    {
        kill_process(*iter, exit_code);
    }
}

static int wait_for_pid(unsigned int pid)
{
#ifdef _MAC_OS
    int kq = kqueue();
    if (-1 == kq)
    {
        return (-1);
    }

    struct kevent kc;
    EV_SET(&kc, pid, EVFILT_PROC, EV_ADD | EV_ENABLE, NOTE_EXIT, 0, 0);

    struct kevent ke;
    int ne = kevent(kq, &kc, 1, &ke, 1, nullptr);
    close(kq);

    if (ne <= 0 || ke.ident != pid || !(ke.fflags & NOTE_EXIT))
    {
        return (-1);
    }

    return (0);
#else
    int ret = -1;
    int in_fd = -1;
    int iw = -1;
    int dir_fd = -1;

    do
    {
        in_fd = inotify_init();
        if (in_fd < 0)
        {
            break;
        }

        char path[32] = { 0x0 };
        snprintf(path, sizeof(path), "/proc/%i/exe", pid);
        iw = inotify_add_watch(in_fd, path, IN_CLOSE_NOWRITE);
        if (iw < 0)
        {
            break;
        }

        snprintf(path, sizeof(path), "/proc/%i", pid);
        dir_fd = open(path, 0);
        if (dir_fd < 0)
        {
            break;
        }

        struct inotify_event event;
        if (read(in_fd, &event, sizeof(event)) < 0)
        {
            break;
        }

        ret = 0;
    } while (false);

    if (dir_fd >= 0)
    {
        close(dir_fd);
    }
    if (in_fd >= 0)
    {
        if (iw >= 0)
        {
            inotify_rm_watch(in_fd, iw);
        }
        close(in_fd);
    }

    return (ret);
#endif // _MAC_OS
}

static int wait_for_child(pid_t pid, int & exit_code)
{
    exit_code = -1;

    int exit_status = -1;
    if (waitpid(pid, &exit_status, 0) != pid)
    {
        return (-1);
    }

    if (WIFEXITED(exit_status))
    {
        exit_code = WEXITSTATUS(exit_status);
    }
    else if (WIFSIGNALED(exit_status))
    {
        exit_code = WTERMSIG(exit_status);
    }
    else if (WIFSTOPPED(exit_status))
    {
        exit_code = WSTOPSIG(exit_status);
    }
    else
    {
        /*
        if (WIFCONTINUED(exit_status))
        {
            return (-1);
        }
        */
        return (-1);
    }

    return (0);
}

NAMESPACE_GOOFER_BEGIN

UnixJoinProcess::UnixJoinProcess(const char * name, bool destroy)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_running(false)
    , m_destroy(destroy)
    , m_locker("thread locker of sub process")
    , m_command_line_params()
{

}

UnixJoinProcess::UnixJoinProcess(const std::string & command_line, const char * name, bool destroy)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_running(false)
    , m_destroy(destroy)
    , m_locker("thread locker of sub process")
    , m_command_line_params()
{
    goofer_split_command_line(command_line.c_str(), m_command_line_params);
}

UnixJoinProcess::UnixJoinProcess(const std::vector<std::string> & command_line_params, const char * name, bool destroy)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_running(false)
    , m_destroy(destroy)
    , m_locker("thread locker of sub process")
    , m_command_line_params(command_line_params)
{

}

UnixJoinProcess::~UnixJoinProcess()
{
    release();
    clear();
}

bool UnixJoinProcess::monitor(unsigned int pid)
{
    if (0 == pid || static_cast<unsigned int>(getpid()) == pid)
    {
        return (false);
    }

    Guard<ThreadLocker> thread_guard(m_locker);
    if (m_running)
    {
        return (false);
    }
    m_running = true;

    char path[32] = { 0x0 };
    snprintf(path, sizeof(path), "/proc/%i/exe", static_cast<int>(pid));
    if (0 != access(path, F_OK))
    {
        m_running = false;
        return (false);
    }

    m_command_line_params.clear();
    m_pid = static_cast<pid_t>(pid);

    return (true);
}

bool UnixJoinProcess::acquire(size_t parent_reader, size_t parent_writer, size_t child_reader, size_t child_writer)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (m_running || m_command_line_params.empty())
    {
        return (false);
    }
    m_running = true;

    clear();

    m_pid = fork();
    if (m_pid < 0)
    {
        m_running = false;
        return (false);
    }
    else if (0 == m_pid)
    {
        std::vector<const char *> argv(m_command_line_params.size() + 1, nullptr);
        for (size_t index = 0; index < m_command_line_params.size(); ++index)
        {
            argv[index] = m_command_line_params[index].c_str();
        }
        if (static_cast<size_t>(-1) != parent_reader)
        {
            close(static_cast<int>(parent_reader));
        }
        if (static_cast<size_t>(-1) != parent_writer)
        {
            close(static_cast<int>(parent_writer));
        }
        if (static_cast<size_t>(-1) != child_reader)
        {
            dup2(static_cast<int>(child_reader), STDIN_FILENO);
            if (STDIN_FILENO != child_reader)
            {
                close(child_reader);
            }
        }
        else
        {
            close(STDIN_FILENO);
        }
        if (static_cast<size_t>(-1) != child_writer)
        {
            dup2(static_cast<int>(child_writer), STDOUT_FILENO);
            dup2(static_cast<int>(child_writer), STDERR_FILENO);
            if (STDOUT_FILENO != child_writer && STDERR_FILENO != child_writer)
            {
                close(child_writer);
            }
        }
        else
        {
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
        }
        /*
         * if execv() success, will not execute the next exit()
         */
        if (execv(argv[0], const_cast<char **>(&argv[0])) < 0)
        {
            exit(0); // should not be here
        }
        exit(0); // should never be here
    }

    return (true);
}

void UnixJoinProcess::release(bool process_tree, int exit_code)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running || 0 == m_pid)
    {
        return;
    }
    m_running = false;

    if (m_destroy)
    {
        goofer_kill_process(m_pid, exit_code, process_tree);
    }
}

bool UnixJoinProcess::wait_exit(int & exit_code)
{
    exit_code = -1;

    if (!m_running || 0 == m_pid)
    {
        return (false);
    }

    bool ret = false;

    const pid_t pid = m_pid;

    if (m_command_line_params.empty())
    {
        ret = (wait_for_pid(static_cast<unsigned int>(pid)) >= 0);
    }
    else
    {
        ret = (wait_for_child(pid, exit_code) >= 0);
    }

    if (pid == m_pid)
    {
        clear();
        m_running = false;
    }
    else
    {
        ret = false;
    }

    return (ret);
}

void UnixJoinProcess::clear()
{
    m_pid = 0;
}

void UnixJoinProcess::set_process_args(const std::string & command_line, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_name = (nullptr != name ? name : "");
        m_command_line_params.clear();
        goofer_split_command_line(command_line.c_str(), m_command_line_params);
    }
}

void UnixJoinProcess::set_process_args(const std::vector<std::string> & command_line_params, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_name = (nullptr != name ? name : "");
        m_command_line_params = command_line_params;
    }
}

bool UnixJoinProcess::running()
{
    return (m_running);
}

std::string UnixJoinProcess::process_name()
{
    return (m_name);
}

unsigned int UnixJoinProcess::process_id() const
{
    return (static_cast<unsigned int>(m_pid));
}

static bool goofer_create_detached_process(const std::vector<std::string> & command_line_params, unsigned int * process_id)
{
    int fd[2] = { -1, -1 };
    if (nullptr != process_id)
    {
        *process_id = 0;
        if (-1 == pipe(fd))
        {
            return (false);
        }
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        if (nullptr != process_id)
        {
            close(fd[0]);
            close(fd[1]);
        }
        return (false);
    }
    else if (0 == pid)
    {
        pid_t cpid = fork();
        if (cpid < 0)
        {
            if (nullptr != process_id)
            {
                close(fd[0]);
                close(fd[1]);
            }
            exit(0);
        }
        else if (0 == cpid)
        {
            if (nullptr != process_id)
            {
                close(fd[0]);
                close(fd[1]);
            }
            std::vector<const char *> argv(command_line_params.size() + 1, nullptr);
            for (size_t index = 0; index < command_line_params.size(); ++index)
            {
                argv[index] = command_line_params[index].c_str();
            }
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            if (execv(argv[0], const_cast<char **>(&argv[0])) < 0)
            {
                exit(0);
            }
            exit(0);
        }
        else
        {
            if (nullptr != process_id)
            {
                *process_id = static_cast<unsigned int>(cpid);
                close(fd[0]);
                write(fd[1], process_id, sizeof(*process_id));
                close(fd[1]);
            }
            exit(0);
        }
    }
    else
    {
        if (nullptr != process_id)
        {
            close(fd[1]);
            read(fd[0], process_id, sizeof(*process_id));
            close(fd[0]);
        }
        waitpid(pid, nullptr, 0);
    }

    return (true);
}

bool goofer_create_detached_process(const std::vector<std::string> & command_line_params)
{
    return (goofer_create_detached_process(command_line_params, nullptr));
}

bool goofer_create_detached_process(const std::vector<std::string> & command_line_params, unsigned int & process_id)
{
    return (goofer_create_detached_process(command_line_params, &process_id));
}

static bool goofer_create_detached_process(const std::string & command_line, unsigned int * process_id)
{
    std::vector<std::string> command_line_params;
    goofer_split_command_line(command_line.c_str(), command_line_params);
    return (goofer_create_detached_process(command_line_params, process_id));
}

bool goofer_create_detached_process(const std::string & command_line)
{
    return (goofer_create_detached_process(command_line, nullptr));
}

bool goofer_create_detached_process(const std::string & command_line, unsigned int & process_id)
{
    return (goofer_create_detached_process(command_line, &process_id));
}

bool goofer_get_process_tree(unsigned int pid, std::list<unsigned int> & pid_list)
{
    return (0 != pid && get_process_tree(pid, pid_list));
}

void goofer_kill_process(unsigned int pid, int exit_code, bool whole_tree)
{
    if (0 == pid)
    {
        return;
    }

    if (whole_tree)
    {
        kill_process_tree(pid, exit_code);
    }
    else
    {
        kill_process(pid, exit_code);
    }
}

NAMESPACE_GOOFER_END


#endif // _MSC_VER
