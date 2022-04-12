/********************************************************
 * Description : log class of sync mode
 * Date        : 2013-05-23 14:02:13
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include "log/sync_log.h"

NAMESPACE_GOOFER_BEGIN

SyncLog::SyncLog(const std::string & path, const std::string & log_type, GOOFER_LOG_LEVEL min_log_level, size_t max_file_size, bool output_to_console)
    : LogBase(path, log_type, min_log_level, max_file_size, output_to_console)
{

}

SyncLog::~SyncLog()
{

}

void SyncLog::save_record(GOOFER_LOG_LEVEL log_level, const char * data, size_t size)
{
    log_level = LOG_LEVEL_MAX; /* prevent warning */
    write(data, size);
}

NAMESPACE_GOOFER_END
