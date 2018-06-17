/********************************************************
 * Description : thread class in unix
 * Data        : 2013-05-23 17:03:46
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_UNIX_THREAD_H
#define GOOFER_UNIX_THREAD_H


#ifndef _MSC_VER


#include <pthread.h>

#include <string>
#include "common/common.h"
#include "utility/uncopy.h"
#include "locker/locker.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API UnixJoinThread : private Uncopy
{
public:
    UnixJoinThread(thread_func_ptr_t func = nullptr, void * argument = nullptr, const char * name = nullptr);
    ~UnixJoinThread();

public:
    bool acquire();
    void release();

public:
    void set_thread_args(thread_func_ptr_t func, void * argument = nullptr, const char * name = nullptr);

public:
    bool running();
    std::string thread_name();
    size_t thread_id() const;

private:
    thread_func_ptr_t   m_func;
    void              * m_argu;
    std::string         m_name;
    pthread_t           m_thread;
    volatile bool       m_running;
    ThreadLocker        m_locker;
};

NAMESPACE_GOOFER_END


#endif // _MSC_VER


#endif // GOOFER_UNIX_THREAD_H
