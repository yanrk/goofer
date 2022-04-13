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

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <unistd.h>
    #include <pthread.h>
#endif // _MSC_VER

#include <cstdlib>
#include "string/string.h"
#include "utility/utility.h"

#ifdef _MSC_VER
    #define ENV_DELIMITER ";"
#else
    #define ENV_DELIMITER ":"
#endif // _MSC_VER

NAMESPACE_GOOFER_BEGIN

size_t get_pid()
{
#ifdef _MSC_VER
    return (static_cast<size_t>(GetCurrentProcessId()));
#else
    return (static_cast<size_t>(getpid()));
#endif // _MSC_VER
}

size_t get_tid()
{
#ifdef _MSC_VER
    return (static_cast<size_t>(GetCurrentThreadId()));
#elif defined(_MAC_OS)
    return (reinterpret_cast<size_t>(pthread_self()));
#else
    return (static_cast<size_t>(pthread_self()));
#endif // _MSC_VER
}

bool get_env(const char * name, std::string & value)
{
    value.clear();
    if (nullptr == name)
    {
        return (false);
    }
    const char * val = getenv(name);
    if (nullptr == val)
    {
        return (false);
    }
    value = val;
    return (true);
}

bool set_env(const char * name, const std::string & value, bool overwrite)
{
    if (nullptr == name)
    {
        return (false);
    }
#if _WIN32
    if (!overwrite && nullptr != getenv(name))
    {
        return (true);
    }
    return (0 == _putenv_s(name, value.c_str()));
#else
    return (0 == setenv(name, value.c_str(), overwrite ? 1 : 0));
#endif
}

bool get_env(const char * name, std::list<std::string> & values)
{
    values.clear();
    std::string value;
    if (!get_env(name, value))
    {
        return (false);
    }
    Goofer::goofer_split_piece(value, ENV_DELIMITER, true, true, values);
    return (true);
}

bool set_env(const char * name, const std::list<std::string> & values, bool overwrite)
{
    std::string value;
    Goofer::goofer_piece_together(values.begin(), values.end(), ENV_DELIMITER, value);
    return (set_env(name, value, overwrite));
}

NAMESPACE_GOOFER_END
