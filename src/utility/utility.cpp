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

#include "utility/utility.h"

#ifdef GOOFER_OS_IS_WIN
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <unistd.h>
    #include <pthread.h>
#endif // GOOFER_OS_IS_WIN

#include <cstdlib>
#include "string/string.h"

NAMESPACE_GOOFER_BEGIN

unsigned int get_pid()
{
#ifdef GOOFER_OS_IS_WIN
    return static_cast<unsigned int>(GetCurrentProcessId());
#else
    return static_cast<unsigned int>(getpid());
#endif // GOOFER_OS_IS_WIN
}

size_t get_tid()
{
#ifdef GOOFER_OS_IS_WIN
    return static_cast<size_t>(GetCurrentThreadId());
#elif defined(GOOFER_OS_IS_MAC)
    return reinterpret_cast<size_t>(pthread_self());
#else
    return static_cast<size_t>(pthread_self());
#endif // GOOFER_OS_IS_WIN
}

bool get_env(const char * name, std::string & value)
{
    value.clear();
    if (nullptr == name)
    {
        return false;
    }
    const char * val = getenv(name);
    if (nullptr == val)
    {
        return false;
    }
    value = val;
    return true;
}

bool set_env(const char * name, const std::string & value, bool overwrite)
{
    if (nullptr == name)
    {
        return false;
    }
#if _WIN32
    if (!overwrite && nullptr != getenv(name))
    {
        return true;
    }
    return 0 == _putenv_s(name, value.c_str());
#else
    return 0 == setenv(name, value.c_str(), overwrite ? 1 : 0);
#endif
}

bool get_env(const char * name, std::list<std::string> & values)
{
    values.clear();
    std::string value;
    if (!get_env(name, value))
    {
        return false;
    }
    Goofer::goofer_split_piece(value, ENV_DELIMITER, true, true, values);
    return true;
}

bool set_env(const char * name, const std::list<std::string> & values, bool overwrite)
{
    std::string value;
    Goofer::goofer_piece_together(values.begin(), values.end(), ENV_DELIMITER, value);
    return set_env(name, value, overwrite);
}

NAMESPACE_GOOFER_END
