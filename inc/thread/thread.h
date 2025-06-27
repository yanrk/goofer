/********************************************************
 * Description : thread class
 * Date        : 2013-05-23 18:02:15
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_THREAD_H
#define GOOFER_THREAD_H


#include "common/common.h"

#ifdef GOOFER_OS_IS_WIN
    #include "thread/windows_thread.h"
#else
    #include "thread/unix_thread.h"
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_BEGIN

#ifdef GOOFER_OS_IS_WIN
    typedef WindowsJoinThread   Thread;
#else
    typedef UnixJoinThread      Thread;
#endif // GOOFER_OS_IS_WIN

GOOFER_CXX_API(bool) goofer_create_detached_thread(thread_func_ptr_t func, thread_argument_t argu);

NAMESPACE_GOOFER_END


#endif // GOOFER_THREAD_H
