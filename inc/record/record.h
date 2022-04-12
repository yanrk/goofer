/********************************************************
 * Description : message record
 * Date        : 2017-03-24 11:30:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_RECORD_H
#define GOOFER_RECORD_H


#include <cstdint>
#include <string>
#include "common/common.h"
#include "locker/locker.h"
#include "filesystem/file.h"
#include "utility/uncopy.h"
#include "utility/singleton.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API Record : private Uncopy
{
public:
    bool init(const std::string & record_root_directory);
    void exit();

public:
    bool running() const;
    void write(const char * data, size_t size, bool success_record);

private:
    void write_success_record(const char * data, size_t size);
    void write_failure_record(const char * data, size_t size);

private:
    Record();
    ~Record();

private:
    friend class Singleton<Record>;

private:
    bool                    m_is_running;
    const int64_t           m_max_filesize;

private:
    std::string             m_success_dirname;
    std::string             m_success_filename;
    File                    m_success_file;
    int64_t                 m_success_filesize;
    ThreadLocker            m_success_file_locker;

private:
    std::string             m_failure_dirname;
    std::string             m_failure_filename;
    File                    m_failure_file;
    int64_t                 m_failure_filesize;
    ThreadLocker            m_failure_file_locker;
};

NAMESPACE_GOOFER_END

GOOFER_CXX_API(void) goofer_record(Goofer::Record & record, bool success_record, const char * format, ...);

#define RECORD_SUCCESS(fmt, ...)    goofer_record(Goofer::Singleton<Goofer::Record>::instance(), true, fmt, ##__VA_ARGS__)

#define RECORD_FAILURE(fmt, ...)    goofer_record(Goofer::Singleton<Goofer::Record>::instance(), false, fmt, ##__VA_ARGS__)


#endif // GOOFER_RECORD_H
