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

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <pthread.h>
#endif // _MSC_VER

#include <fstream>
#include "utility/guard.h"
#include "utility/utility.h"
#include "filesystem/directory.h"
#include "time/time.h"
#include "string/string.h"
#include "log/log_base.h"

NAMESPACE_GOOFER_BEGIN

LogBase::LogBase(const std::string & path, const std::string & log_type, const GOOFER_LOG_CONFIG::FILE_CONFIG & file_config)
    : m_dirname(path + g_directory_separator + log_type + g_directory_separator)
    , m_log_type(log_type)
    , m_filename(m_dirname + m_log_type + ".log")
    , m_filelist()
    , m_old_date()
    , m_file()
    , m_min_log_level(file_config.log_min_level)
    , m_cur_file_size(0)
    , m_max_file_size(file_config.log_file_size * 1024 * 1024)
    , m_cur_file_count(0)
    , m_file_locker("file locker of base log")
    , m_output_to_console(file_config.output_to_console)
    , m_auto_delete_file(file_config.auto_delete_file)
    , m_max_file_count(file_config.max_keep_count)
    , m_max_keep_days(file_config.max_keep_days)
{
    goofer_create_directory_recursive(m_dirname);
    m_file.open(m_filename.c_str(), true, false);
    if (!file_get_size(m_filename.c_str(), m_cur_file_size))
    {
        m_cur_file_size = 0;
    }
    m_old_date = Goofer::goofer_get_date("");

    load_files();
    clean_files();
}

LogBase::~LogBase()
{
    m_file.close();
}

void LogBase::push_record(GOOFER_LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, va_list args)
{
    if (nullptr == file || nullptr == func || nullptr == format || nullptr == args)
    {
        return;
    }

    if (level > m_min_log_level)
    {
        return;
    }

    char record[LOG_RECORD_SIZE];
    size_t record_size = 0;

    record_size += goofer_snprintf
    (
        record + record_size, LOG_RECORD_SIZE - record_size, 
        "%s", goofer_get_comprehensive_datetime("-", ":", " ", true).c_str()
    );

    record_size += goofer_snprintf
    (
        record + record_size, LOG_RECORD_SIZE - record_size, 
        " | %s | T%010u | %s:%s:%05d | ", 
        GOOFER_LOG_LEVEL_INFO[level], get_tid(), file, func, line
    );

    record_size += goofer_vsnprintf
    (
        record + record_size, LOG_RECORD_SIZE - record_size, format, args
    );

    record_size += 1; /* for '\n' or '\0' */
    if (LOG_RECORD_SIZE < record_size)
    {
        record_size = LOG_RECORD_SIZE;
    }

    record[record_size - 1] = '\0';

#ifdef _MSC_VER
//  OutputDebugStringA(record);
#endif // _MSC_VER

    if (m_output_to_console)
    {
        printf("%s\n", record);
    }

    record[record_size - 1] = '\n';
    save_record(level, record, record_size);
}

void LogBase::set_min_level(GOOFER_LOG_LEVEL level)
{
    m_min_log_level = level;
}

void LogBase::set_console_output_switch(bool output_to_console)
{
    m_output_to_console = output_to_console;
}

void LogBase::write(const char * data, size_t size)
{
    Guard<ThreadLocker> file_guard(m_file_locker);

    if (!m_file.write(data, size))
    {
        return;
    }

#ifndef _MSC_VER
    m_file.flush();
#endif // _MSC_VER

    m_cur_file_size += size;
    if (m_cur_file_size < m_max_file_size)
    {
        return;
    }

    m_file.close();

    const std::string date_time = goofer_get_datetime("", "", "");
    const std::string new_date = date_time.substr(0, 8);

    const std::string date_dirname(m_dirname + new_date + g_directory_separator);
    goofer_mkdir_safe(date_dirname.c_str());

    const std::string filename(m_log_type + "_" + date_time + ".log");
    const std::string pathname(date_dirname + filename);
    goofer_rename_safe(m_filename.c_str(), pathname.c_str());

    m_file.open(m_filename.c_str(), true, true);
    m_cur_file_size = 0;

    if (m_auto_delete_file)
    {
        if (m_old_date != new_date)
        {
            m_filelist.push_back(m_old_date);
            m_old_date = new_date;
        }

        m_filelist.push_back(new_date + g_directory_separator + filename);
        ++m_cur_file_count;

        clean_files();
    }
}

void LogBase::load_files()
{
    if (!m_auto_delete_file)
    {
        return;
    }

    Goofer::Directory dir;
    if (!dir.open(m_dirname.c_str()))
    {
        return;
    }

    while (dir.read())
    {
        if (dir.sub_path_is_directory() && dir.sub_path_short_name().size() == 8)
        {
            Goofer::Directory sub_dir;
            if (sub_dir.open(dir.sub_path_name().c_str()))
            {
                while (sub_dir.read())
                {
                    if (!sub_dir.sub_path_is_directory())
                    {
                        m_filelist.push_back(dir.sub_path_short_name() + g_directory_separator + sub_dir.sub_path_short_name());
                        ++m_cur_file_count;
                    }
                }
                sub_dir.close();
            }
            if (dir.sub_path_short_name() != m_old_date)
            {
                m_filelist.push_back(dir.sub_path_short_name());
            }
        }
    }

    dir.close();
}

void LogBase::clean_files()
{
    if (!m_auto_delete_file)
    {
        return;
    }

    const std::string min_file_date = Goofer::goofer_get_date(Goofer::goofer_make_localtime(Goofer::goofer_time() - m_max_keep_days * 3600 * 24), "");
    while (!m_filelist.empty())
    {
        const std::string & pathname = m_filelist.front();
        if (pathname.substr(0, 8) > min_file_date && m_cur_file_count < m_max_file_count)
        {
            break;
        }

        const std::string fullpath = m_dirname + pathname;
        if (pathname.size() == 8)
        {
            Goofer::goofer_remove_directory_recursive(fullpath);
        }
        else
        {
            Goofer::goofer_unlink_safe(fullpath.c_str());
            --m_cur_file_count;
        }
        m_filelist.pop_front();
    }
}

NAMESPACE_GOOFER_END
