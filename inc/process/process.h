/********************************************************
 * Description : process class
 * Date        : 2016-09-09 10:13:16
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_PROCESS_H
#define GOOFER_PROCESS_H


#include <string>
#include <vector>
#include "common/common.h"

#ifdef GOOFER_OS_IS_WIN
    #include "process/windows_process.h"
#else
    #include "process/unix_process.h"
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_BEGIN

#ifdef GOOFER_OS_IS_WIN
    typedef WindowsJoinProcess   Process;
#else
    typedef UnixJoinProcess      Process;
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_END


#endif // GOOFER_PROCESS_H
