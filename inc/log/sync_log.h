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

#ifndef GOOFER_SYNC_LOG_H
#define GOOFER_SYNC_LOG_H


#include "common/common.h"
#include "log/log_base.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API SyncLog : public LogBase
{
public:
    SyncLog(const std::string & path, const std::string & log_type, const GOOFER_LOG_CONFIG::FILE_CONFIG & file_config);
    virtual ~SyncLog();

protected:
    virtual void save_record(GOOFER_LOG_LEVEL log_level, const char * data, size_t size);
};

NAMESPACE_GOOFER_END


#endif // GOOFER_SYNC_LOG_H
