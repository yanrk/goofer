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
#include <list>
#include "common/common.h"
#include "utility/uncopy.h"
#include "filesystem/file.h"
#include "locker/locker.h"
#include "log/log_types.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API LogBase : private Uncopy
{
public:
    LogBase(const std::string & path, const std::string & log_type, const GOOFER_LOG_CONFIG::FILE_CONFIG & file_config);
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
    void load_files();
    void clean_files();

private:
    const std::string       m_dirname;
    const std::string       m_log_type;
    const std::string       m_filename;
    std::list<std::string>  m_filelist;
    std::string             m_old_date;
    File                    m_file;
    GOOFER_LOG_LEVEL        m_min_log_level;
    int64_t                 m_cur_file_size;
    const int64_t           m_max_file_size;
    size_t                  m_cur_file_count;
    ThreadLocker            m_file_locker;
    bool                    m_output_to_console;
    bool                    m_auto_delete_file;
    size_t                  m_max_file_count;
    size_t                  m_max_keep_days;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_LOG_BASE_H
