/********************************************************
 * Description : base class of log classes
 * Date        : 2013-05-23 09:24:56
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_LOG_BASE_H
#define GOOFER_LOG_BASE_H


#include <cstdarg>
#include <string>
#include "common/common.h"
#include "utility/uncopy.h"
#include "filesystem/file.h"
#include "locker/locker.h"
#include "log/log_types.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API LogBase : private Uncopy
{
public:
    LogBase(const std::string & path, const std::string & log_type, GOOFER_LOG_LEVEL min_log_level, size_t max_file_size, bool output_to_console);
    virtual ~LogBase();

public:
    void push_record(GOOFER_LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, va_list args);
    void set_min_level(GOOFER_LOG_LEVEL level);
    void set_console_output_switch(bool output_to_console);

protected:
    virtual void save_record(GOOFER_LOG_LEVEL log_level, const char * data, size_t size) = 0;

protected:
    void write(const char * data, size_t size);

private:
    const std::string   m_dirname;
    const std::string   m_log_type;
    const std::string   m_filename;
    File                m_file;
    GOOFER_LOG_LEVEL    m_min_log_level;
    int64_t             m_cur_file_size;
    const int64_t       m_max_file_size;
    ThreadLocker        m_file_locker;
    bool                m_output_to_console;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_LOG_BASE_H
