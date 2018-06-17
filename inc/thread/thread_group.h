/********************************************************
 * Description : thread group class
 * Data        : 2015-07-02 09:57:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_THREAD_GROUP_H
#define GOOFER_THREAD_GROUP_H


#include <list>
#include "common/common.h"
#include "utility/uncopy.h"
#include "locker/locker.h"
#include "thread/thread.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API ThreadGroup : private Uncopy
{
public:
    ThreadGroup();
    ~ThreadGroup();

public:
    size_t size();
    bool acquire_thread(thread_func_ptr_t func, void * param, const char * name = nullptr);
    void release_threads();

private:
    std::list<Thread *>     m_thread_list;
    ThreadLocker            m_thread_locker;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_THREAD_GROUP_H
