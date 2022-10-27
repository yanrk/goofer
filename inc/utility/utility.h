/********************************************************
 * Description : utility of base
 * Date        : 2015-02-28 10:38:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_UTILITY_H
#define GOOFER_UTILITY_H


#include <list>
#include <string>
#include "common/common.h"

NAMESPACE_GOOFER_BEGIN

GOOFER_CXX_API(unsigned int) get_pid();
GOOFER_CXX_API(size_t) get_tid();
GOOFER_CXX_API(bool) get_env(const char * name, std::string & value);
GOOFER_CXX_API(bool) set_env(const char * name, const std::string & value, bool overwrite = true);
GOOFER_CXX_API(bool) get_env(const char * name, std::list<std::string> & values);
GOOFER_CXX_API(bool) set_env(const char * name, const std::list<std::string> & values, bool overwrite = true);

NAMESPACE_GOOFER_END


#endif // GOOFER_UTILITY_H
