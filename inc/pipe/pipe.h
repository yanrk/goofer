/********************************************************
 * Description : pipe class
 * Data        : 2018-11-26 19:36:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_PIPE_H
#define GOOFER_PIPE_H


#include "common/common.h"

#ifdef _MSC_VER
    #include "pipe/windows_fifo.h"
    #include "pipe/windows_pipe.h"
#else
    #include "pipe/unix_fifo.h"
    #include "pipe/unix_pipe.h"
#endif // _MSC_VER

#include <cstdio>

#ifdef _MSC_VER
    #define goofer_popen(cmd, mode)     ::_popen(cmd, mode)
    #define goofer_pclose(file)         ::_pclose(file)
#else
    #define goofer_popen(cmd, mode)     ::popen(cmd, mode)
    #define goofer_pclose(file)         ::pclose(file)
#endif // _MSC_VER

NAMESPACE_GOOFER_BEGIN

#ifdef _MSC_VER
    typedef WindowsAnonymousPipe        AnonymousPipe;
    typedef WindowsNamedPipe            NamedPipe;
#else
    typedef UnixAnonymousPipe           AnonymousPipe;
    typedef UnixNamedPipe               NamedPipe;
#endif // _MSC_VER

NAMESPACE_GOOFER_END


#endif // GOOFER_PIPE_H
