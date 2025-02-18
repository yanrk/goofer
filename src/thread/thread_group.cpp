/********************************************************
 * Description : thread group class
 * Date        : 2015-07-02 09:57:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include "thread/thread_group.h"
#include "utility/guard.h"

NAMESPACE_GOOFER_BEGIN

ThreadGroup::ThreadGroup()
    : m_thread_list()
    , m_thread_locker()
{

}

ThreadGroup::~ThreadGroup()
{
    release_threads();
}

size_t ThreadGroup::size()
{
    Guard<ThreadLocker> thread_guard(m_thread_locker);
    return (m_thread_list.size());
}

bool ThreadGroup::acquire_thread(thread_func_ptr_t func, void * param, const char * name)
{
    Thread * thread = nullptr;
    GOOFER_NEW(thread, Thread);
    if (nullptr != thread)
    {
        thread->set_thread_args(func, param, name);
        if (thread->acquire())
        {
            Guard<ThreadLocker> thread_guard(m_thread_locker);
            m_thread_list.push_back(thread);
        }
        else
        {
            GOOFER_DEL(thread);
        }
    }
    return (nullptr != thread);
}

void ThreadGroup::release_threads()
{
    Guard<ThreadLocker> thread_guard(m_thread_locker);
    for (std::list<Thread *>::iterator iter = m_thread_list.begin(); m_thread_list.end() != iter; ++iter)
    {
        Thread * thread = *iter;
        thread->release();
        GOOFER_DEL(thread);
    }
    m_thread_list.clear();
}

NAMESPACE_GOOFER_END
