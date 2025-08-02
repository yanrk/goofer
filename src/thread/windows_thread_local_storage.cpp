/********************************************************
 * Description : thread local storage class in windows
 * Date        : 2015-02-27 16:01:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include "thread/windows_thread_local_storage.h"

#ifdef GOOFER_OS_IS_WIN


#include <cassert>

NAMESPACE_GOOFER_BEGIN

WindowsThreadLocalStorage::WindowsThreadLocalStorage()
    : m_thread_key(TlsAlloc())
{
    assert(TLS_OUT_OF_INDEXES != m_thread_key);
}

WindowsThreadLocalStorage::~WindowsThreadLocalStorage()
{
    TlsFree(m_thread_key);
}

void WindowsThreadLocalStorage::set_value(void * thread_specific_value)
{
    TlsSetValue(m_thread_key, thread_specific_value);
}

void * WindowsThreadLocalStorage::get_value()
{
    return TlsGetValue(m_thread_key);
}

NAMESPACE_GOOFER_END


#endif // GOOFER_OS_IS_WIN
