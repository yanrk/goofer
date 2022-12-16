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

#ifndef GOOFER_LAZY_LOG_H
#define GOOFER_LAZY_LOG_H


#include "common/common.h"
#include "locker/locker.h"
#include "log/log_base.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API LazyLog : public LogBase
{
public:
    LazyLog(const std::string & path, const std::string & log_type, const GOOFER_LOG_CONFIG::FILE_CONFIG & file_config);
    virtual ~LazyLog();

protected:
    virtual void save_record(GOOFER_LOG_LEVEL log_level, const char * data, size_t size);

private:
    void flush();

private:
    struct LOG_RECORD
    {
        char   data[LOG_RECORD_SIZE];
        size_t size;
    };

private:
    LOG_RECORD     * m_record;
    size_t           m_reserve;
    size_t           m_begin;
    size_t           m_count;
    ThreadLocker     m_record_locker;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_LAZY_LOG_H
