/********************************************************
 * Description : thread class
 * Data        : 2013-05-23 18:02:15
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

#ifdef _MSC_VER
    #include "thread/windows_thread.h"
#else
    #include "thread/unix_thread.h"
#endif // _MSC_VER

NAMESPACE_GOOFER_BEGIN

#ifdef _MSC_VER
    typedef WindowsJoinThread   Thread;
#else
    typedef UnixJoinThread      Thread;
#endif // _MSC_VER

GOOFER_CXX_API(bool) goofer_create_detached_thread(thread_func_ptr_t func, thread_argument_t argu);

NAMESPACE_GOOFER_END


#endif // GOOFER_THREAD_H
