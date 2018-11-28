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
    #include "pipe/windows_pipe.h"
#else
    #include "pipe/unix_pipe.h"
#endif // _MSC_VER

NAMESPACE_GOOFER_BEGIN

#ifdef _MSC_VER
    typedef WindowsAnonymousPipe    AnonymousPipe;
#else
    typedef UnixAnonymousPipe       AnonymousPipe;
#endif // _MSC_VER

NAMESPACE_GOOFER_END


#endif // GOOFER_PIPE_H
