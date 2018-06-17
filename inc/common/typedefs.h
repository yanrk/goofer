/********************************************************
 * Description : typedefs of goofer
 * Data        : 2013-05-18 17:43:12
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_TYPEDEFS_H
#define GOOFER_TYPEDEFS_H


#include "common/macros.h"

#ifdef _MSC_VER
    typedef unsigned int         thread_return_t;
    typedef void *               thread_argument_t;
    #define THREAD_DEFAULT_RET   0U
#else
    typedef void *               thread_return_t;
    typedef void *               thread_argument_t;
    #define THREAD_DEFAULT_RET   nullptr
#endif // _MSC_VER

typedef thread_return_t (GOOFER_STDCALL * thread_func_ptr_t) (thread_argument_t);


#endif // GOOFER_TYPEDEFS_H
