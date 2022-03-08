/********************************************************
 * Description : process class in windows
 * Data        : 2016-09-09 16:38:54
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_WINDOWS_PROCESS_H
#define GOOFER_WINDOWS_PROCESS_H


#ifdef _MSC_VER


#include <windows.h>

#include <string>
#include <vector>
#include <list>
#include "common/common.h"
#include "utility/uncopy.h"
#include "locker/locker.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API WindowsJoinProcess : private Uncopy
{
public:
    explicit WindowsJoinProcess(const char * name = nullptr, bool destroy = true);
    explicit WindowsJoinProcess(const std::string & command_line, const char * name = nullptr, bool destroy = true);
    explicit WindowsJoinProcess(const std::vector<std::string> & command_line_params, const char * name = nullptr, bool destroy = true);
    ~WindowsJoinProcess();

public:
    bool monitor(size_t pid);
    bool acquire(size_t parent_reader = reinterpret_cast<size_t>(INVALID_HANDLE_VALUE), size_t parent_writer = reinterpret_cast<size_t>(INVALID_HANDLE_VALUE), size_t child_reader = reinterpret_cast<size_t>(INVALID_HANDLE_VALUE), size_t child_writer = reinterpret_cast<size_t>(INVALID_HANDLE_VALUE));
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
    DWORD                       m_pid;
    HANDLE                      m_handle;
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


#endif // GOOFER_WINDOWS_PROCESS_H
