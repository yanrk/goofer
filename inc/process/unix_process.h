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

#ifndef GOOFER_UNIX_PROCESS_H
#define GOOFER_UNIX_PROCESS_H


#ifndef _MSC_VER


#include <pthread.h>

#include <string>
#include <vector>
#include <list>
#include "common/common.h"
#include "utility/uncopy.h"
#include "locker/locker.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API UnixJoinProcess : private Uncopy
{
public:
    explicit UnixJoinProcess(const char * name = nullptr, bool destroy = true);
    explicit UnixJoinProcess(const std::string & command_line, const char * name = nullptr, bool destroy = true);
    explicit UnixJoinProcess(const std::vector<std::string> & command_line_params, const char * name = nullptr, bool destroy = true);
    ~UnixJoinProcess();

public:
    bool monitor(size_t pid);
    bool acquire(size_t parent_reader = static_cast<size_t>(-1), size_t parent_writer = static_cast<size_t>(-1), size_t child_reader = static_cast<size_t>(-1), size_t child_writer = static_cast<size_t>(-1));
    void release(bool process_tree = false, int exit_code = 9);
    bool wait_exit(int & exit_code);

public:
    void set_process_args(const std::string & command_line, const char * name = nullptr);
    void set_process_args(const std::vector<std::string> & command_line_params, const char * name = nullptr);

public:
    bool running();
    std::string process_name();
    size_t process_id() const;

private:
    void clear();

private:
    std::string                 m_name;
    pid_t                       m_pid;
    volatile bool               m_running;
    const bool                  m_destroy;
    ThreadLocker                m_locker;
    std::vector<std::string>    m_command_line_params;
};

GOOFER_CXX_API(bool) goofer_create_detached_process(const std::string & command_line);
GOOFER_CXX_API(bool) goofer_create_detached_process(const std::vector<std::string> & command_line_params);
GOOFER_CXX_API(bool) goofer_get_process_tree(size_t pid, std::list<size_t> & pid_list);
GOOFER_CXX_API(void) goofer_kill_process(size_t pid, int exit_code = 9, bool whole_tree = false);

NAMESPACE_GOOFER_END


#endif // _MSC_VER


#endif // GOOFER_UNIX_PROCESS_H
