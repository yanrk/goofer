/********************************************************
 * Description : constants of goofer
 * Data        : 2014-01-06 10:29:46
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_CONSTS_H
#define GOOFER_CONSTS_H


#include "common/macros.h"
#include "common/namespace.h"

NAMESPACE_GOOFER_BEGIN

#ifdef _MSC_VER
    #define g_directory_separator   '\\'
#else
    #define g_directory_separator   '/'
#endif // _MSC_VER

#define g_blank_character_set       " \t\v\r\n\f"

NAMESPACE_GOOFER_END


#endif // GOOFER_CONSTS_H
