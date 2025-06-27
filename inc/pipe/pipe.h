/********************************************************
 * Description : pipe class
 * Date        : 2018-11-26 19:36:00
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

#ifdef GOOFER_OS_IS_WIN
    #include "pipe/windows_fifo.h"
    #include "pipe/windows_pipe.h"
#else
    #include "pipe/unix_fifo.h"
    #include "pipe/unix_pipe.h"
#endif // GOOFER_OS_IS_WIN

#include <cstdio>

NAMESPACE_GOOFER_BEGIN

#ifdef GOOFER_OS_IS_WIN
    typedef WindowsAnonymousPipe        AnonymousPipe;
    typedef WindowsNamedPipe            NamedPipe;
#else
    typedef UnixAnonymousPipe           AnonymousPipe;
    typedef UnixNamedPipe               NamedPipe;
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_END


#endif // GOOFER_PIPE_H
