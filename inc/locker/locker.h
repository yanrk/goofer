/********************************************************
 * Description : locker classes
 * Date        : 2013-05-20 08:03:27
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_LOCKER_H
#define GOOFER_LOCKER_H


#include "locker/null_locker.h"

#ifdef GOOFER_OS_IS_WIN
    #include "locker/windows_locker.h"
#else
    #include "locker/unix_locker.h"
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_BEGIN

#ifdef GOOFER_OS_IS_WIN
    typedef WindowsThreadLocker      ThreadLocker;
    typedef WindowsProcessLocker     ProcessLocker;
#else
    typedef UnixMutexLocker<true>    ThreadLocker;
    typedef UnixMutexLocker<false>   ProcessLocker;
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_END


#endif // GOOFER_LOCKER_H
