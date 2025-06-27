/********************************************************
 * Description : thread local storage class
 * Date        : 2015-02-27 16:18:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_THREAD_LOCAL_STORAGE_H
#define GOOFER_THREAD_LOCAL_STORAGE_H


#include "common/common.h"

#ifdef GOOFER_OS_IS_WIN
    #include "thread/windows_thread_local_storage.h"
#else
    #include "thread/unix_thread_local_storage.h"
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_BEGIN

#ifdef GOOFER_OS_IS_WIN
    typedef WindowsThreadLocalStorage   ThreadLocalStorage;
#else
    typedef UnixThreadLocalStorage      ThreadLocalStorage;
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_END


#endif // GOOFER_THREAD_LOCAL_STORAGE_H
