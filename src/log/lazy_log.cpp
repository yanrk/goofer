/********************************************************
 * Description : log class of lazy mode
 * Date        : 2013-05-23 13:56:44
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cassert>
#include <cstring>
#include "utility/guard.h"
#include "log/lazy_log.h"

NAMESPACE_GOOFER_BEGIN

LazyLog::LazyLog(const std::string & path, const std::string & log_type, const GOOFER_LOG_CONFIG::FILE_CONFIG & file_config)
    : LogBase(path, log_type, file_config)
    , m_record(nullptr)
    , m_reserve(file_config.buffer_count)
    , m_begin(0)
    , m_count(0)
    , m_record_locker("record locker of lazy log")
{
    GOOFER_NEW_ARR(m_record, LOG_RECORD, file_config.buffer_count);
    assert(nullptr != m_record);
}

LazyLog::~LazyLog()
{
    GOOFER_DEL_ARR(m_record);
}

void LazyLog::save_record(GOOFER_LOG_LEVEL log_level, const char * data, size_t size)
{
    Guard<ThreadLocker> record_guard(m_record_locker);

    if (nullptr == m_record)
    {
        return;
    }

    if (size > LOG_RECORD_SIZE)
    {
        size = LOG_RECORD_SIZE;
    }

    size_t push_index = (m_begin + m_count) % m_reserve;
    memcpy(m_record[push_index].data, data, size);
    m_record[push_index].size = size;
    m_record[push_index].data[size - 1] = '\n';

    if (m_count < m_reserve)
    {
        ++m_count;
    }

    if (ERR_LEVEL >= log_level)
    {
        flush();
    }
}

void LazyLog::flush()
{
    for (size_t index = 0; index < m_count; ++index)
    {
        size_t record_index = (m_begin + index) % m_reserve;
        write(m_record[record_index].data, m_record[record_index].size);
    }
    m_begin = 0;
    m_count = 0;
}

NAMESPACE_GOOFER_END
