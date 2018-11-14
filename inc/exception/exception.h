/********************************************************
 * Description : exception class
 * Data        : 2013-05-20 06:56:52
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_EXCEPTION_H
#define GOOFER_EXCEPTION_H


#include <string>
#include "common/common.h"
#include "string/string.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API Exception
{
public:
    Exception(const char * file, int line, const char * func, const char * msg);

public:
    const std::string & what() const;

protected:
    std::string   m_message;
};

#define GOOFER_THROW(fmt, ...)                                                \
do                                                                            \
{                                                                             \
    const size_t max_message_length = 1024;                                   \
    char message[max_message_length] = { 0x0 };                               \
    Goofer::goofer_snprintf(message, max_message_length, fmt, ##__VA_ARGS__); \
    throw Goofer::Exception(__FILE__, __LINE__, __FUNCTION__, message);       \
} while (false)

GOOFER_CXX_API(void) goofer_set_dump_directory(const char * store_dir_root, const char * store_file_prefix);

NAMESPACE_GOOFER_END


#endif // GOOFER_EXCEPTION_H
