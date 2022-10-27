/********************************************************
 * Description : process class in windows
 * Date        : 2016-09-09 16:38:54
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER


#include <windows.h>
#include <tlhelp32.h>
#include "process/windows_process.h"
#include "string/string.h"
#include "utility/guard.h"

struct process_info_t
{
    DWORD   pid;
    DWORD   ppid;
};

static void get_all_process(std::list<process_info_t> & process_info_list)
{
    process_info_list.clear();

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == snapshot)
    {
        return;
    }

    PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };

    for (BOOL ok = Process32First(snapshot, &pe); TRUE == ok; ok = Process32Next(snapshot, &pe))
    {
        process_info_t process_info;
        process_info.pid = pe.th32ProcessID;
        process_info.ppid = pe.th32ParentProcessID;
        process_info_list.push_back(process_info);
    }

    CloseHandle(snapshot);
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

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, static_cast<DWORD>(process_id));
    if (nullptr == process)
    {
        return;
    }

    UINT exit_status = static_cast<UINT>(exit_code);
    TerminateProcess(process, exit_status);
    CloseHandle(process);
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

static void command_params_to_command_line(const std::vector<std::string> & command_params, std::string & command_line)
{
    command_line.clear();
    for (std::vector<std::string>::const_iterator iter = command_params.begin(); command_params.end() != iter; ++iter)
    {
        const std::string & param = *iter;
        if (command_params.begin() != iter)
        {
            command_line += " ";
        }
        if ((param.empty()) || ('\"' != param[0] && std::string::npos != param.find(' ')))
        {
            command_line += '\"' + param + '\"';
        }
        else
        {
            command_line += param;
        }
    }
}

NAMESPACE_GOOFER_BEGIN

WindowsJoinProcess::WindowsJoinProcess(const char * name, bool destroy)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_handle(nullptr)
    , m_running(false)
    , m_destroy(destroy)
    , m_locker("thread locker of sub process")
    , m_command_line_params()
{

}

WindowsJoinProcess::WindowsJoinProcess(const std::string & command_line, const char * name, bool destroy)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_handle(nullptr)
    , m_running(false)
    , m_destroy(destroy)
    , m_locker("thread locker of sub process")
    , m_command_line_params()
{
    goofer_split_command_line(command_line.c_str(), m_command_line_params);
}

WindowsJoinProcess::WindowsJoinProcess(const std::vector<std::string> & command_line_params, const char * name, bool destroy)
    : m_name(nullptr != name ? name : "")
    , m_pid(0)
    , m_handle(nullptr)
    , m_running(false)
    , m_destroy(destroy)
    , m_locker("thread locker of sub process")
    , m_command_line_params(command_line_params)
{

}

WindowsJoinProcess::~WindowsJoinProcess()
{
    release();
    clear();
}

bool WindowsJoinProcess::monitor(unsigned int pid)
{
    if (0 == pid || GetCurrentProcessId() == pid)
    {
        return (false);
    }

    Guard<ThreadLocker> thread_guard(m_locker);
    if (m_running)
    {
        return (false);
    }
    m_running = true;

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, static_cast<DWORD>(pid));
    if (nullptr == handle)
    {
        m_running = false;
        return (false);
    }

    m_command_line_params.clear();
    m_pid = static_cast<DWORD>(pid);
    m_handle = handle;

    return (true);
}

bool WindowsJoinProcess::acquire(size_t parent_reader, size_t parent_writer, size_t child_reader, size_t child_writer)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (m_running || m_command_line_params.empty())
    {
        return (false);
    }
    m_running = true;

    std::string command_line;
    command_params_to_command_line(m_command_line_params, command_line);

    clear();

    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    if (reinterpret_cast<size_t>(INVALID_HANDLE_VALUE) != parent_reader)
    {
        SetHandleInformation(reinterpret_cast<HANDLE>(parent_reader), HANDLE_FLAG_INHERIT, 0);
    }
    if (reinterpret_cast<size_t>(INVALID_HANDLE_VALUE) != parent_writer)
    {
        SetHandleInformation(reinterpret_cast<HANDLE>(parent_writer), HANDLE_FLAG_INHERIT, 0);
    }
    if (reinterpret_cast<size_t>(INVALID_HANDLE_VALUE) != child_reader)
    {
        si.hStdInput = reinterpret_cast<HANDLE>(child_reader);
        si.dwFlags |= STARTF_USESTDHANDLES;
    }
    if (reinterpret_cast<size_t>(INVALID_HANDLE_VALUE) != child_writer)
    {
        si.hStdOutput = reinterpret_cast<HANDLE>(child_writer);
        si.hStdError = reinterpret_cast<HANDLE>(child_writer);
        si.dwFlags |= STARTF_USESTDHANDLES;
    }
    PROCESS_INFORMATION pi = { 0x0 };

    if (!CreateProcessA(nullptr, reinterpret_cast<LPSTR>(const_cast<char *>(command_line.c_str())), nullptr, nullptr, 0 != si.dwFlags, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi))
    {
        m_running = false;
        return (false);
    }
    CloseHandle(pi.hThread);

    m_pid = pi.dwProcessId;
    m_handle = pi.hProcess;

    return (true);
}

void WindowsJoinProcess::release(bool process_tree, int exit_code)
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

bool WindowsJoinProcess::wait_exit(int & exit_code)
{
    if (!m_running || nullptr == m_handle)
    {
        return (false);
    }

    bool ret = false;

    const DWORD pid = m_pid;

    do
    {
        exit_code = -1;

        if (WAIT_OBJECT_0 != WaitForSingleObject(m_handle, INFINITE))
        {
            break;
        }

        DWORD exit_status = ~static_cast<DWORD>(0);
        if (!GetExitCodeProcess(m_handle, &exit_status))
        {
            break;
        }

        /*
        if (STILL_ACTIVE == exit_status)
        {
            break;
        }
        */

        exit_code = static_cast<int>(exit_status);

        ret = true;
    } while (false);

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

void WindowsJoinProcess::clear()
{
    if (nullptr != m_handle)
    {
        CloseHandle(m_handle);
    }
    m_pid = 0;
    m_handle = nullptr;
}

void WindowsJoinProcess::set_process_args(const std::string & command_line, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_name = (nullptr != name ? name : "");
        m_command_line_params.clear();
        goofer_split_command_line(command_line.c_str(), m_command_line_params);
    }
}

void WindowsJoinProcess::set_process_args(const std::vector<std::string> & command_line_params, const char * name)
{
    Guard<ThreadLocker> thread_guard(m_locker);
    if (!m_running)
    {
        m_name = (nullptr != name ? name : "");
        m_command_line_params = command_line_params;
    }
}

bool WindowsJoinProcess::running()
{
    return (m_running);
}

std::string WindowsJoinProcess::process_name()
{
    return (m_name);
}

unsigned int WindowsJoinProcess::process_id() const
{
    return (static_cast<unsigned int>(m_pid));
}

static bool goofer_create_detached_process(const std::string & command_line, unsigned int * process_id)
{
    if (nullptr != process_id)
    {
        *process_id = 0;
    }

    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi = { 0x0 };

    if (!CreateProcessA(nullptr, reinterpret_cast<LPSTR>(const_cast<char *>(command_line.c_str())), nullptr, nullptr, false, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi))
    {
        return (false);
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    if (nullptr != process_id)
    {
        *process_id = pi.dwProcessId;
    }

    return (true);
}

bool goofer_create_detached_process(const std::string & command_line)
{
    return (goofer_create_detached_process(command_line, nullptr));
}

bool goofer_create_detached_process(const std::string & command_line, unsigned int & process_id)
{
    return (goofer_create_detached_process(command_line, &process_id));
}

static bool goofer_create_detached_process(const std::vector<std::string> & command_line_params, unsigned int * process_id)
{
    std::string command_line;
    command_params_to_command_line(command_line_params, command_line);
    return (goofer_create_detached_process(command_line, process_id));
}

bool goofer_create_detached_process(const std::vector<std::string> & command_line_params)
{
    return (goofer_create_detached_process(command_line_params, nullptr));
}

bool goofer_create_detached_process(const std::vector<std::string> & command_line_params, unsigned int & process_id)
{
    return (goofer_create_detached_process(command_line_params, &process_id));
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
