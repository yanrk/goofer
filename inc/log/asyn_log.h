/********************************************************
 * Description : log class of asyn mode
 * Date        : 2013-05-23 20:12:41
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_ASYN_LOG_H
#define GOOFER_ASYN_LOG_H


#include <list>
#include <vector>
#include "common/common.h"
#include "thread/thread.h"
#include "locker/locker.h"
#include "pool/object_pool.h"
#include "log/log_base.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API AsynLog : public LogBase
{
public:
    AsynLog(const std::string & path, const std::string & log_type, const GOOFER_LOG_CONFIG::FILE_CONFIG & file_config);
    virtual ~AsynLog();

public:
    static bool acquire_write_thread();
    static void release_write_thread();

    static void push_asyn_log_object(AsynLog * asyn_log);
    static void pop_asyn_log_object(AsynLog * asyn_log);

    static void write_file();

protected:
    virtual void save_record(GOOFER_LOG_LEVEL log_level, const char * data, size_t size);

private:
    static bool try_write_file();

private:
    enum { ASYN_RECORD_SIZE = LOG_RECORD_SIZE * 8 };

    struct LOG_RECORD
    {
        char   data[ASYN_RECORD_SIZE];
        size_t size;
    };

private:
    static Thread                            s_write_file_thread;
    static std::list<AsynLog *>              s_asyn_log_obj_list;
    static ThreadLocker                      s_asyn_log_obj_locker;

private:
    ObjectPool<LOG_RECORD, ThreadLocker>     m_idle_pool;
    ObjectPool<LOG_RECORD, ThreadLocker>     m_data_pool;
    LOG_RECORD                             * m_current_record;
    ThreadLocker                             m_current_record_locker;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_ASYN_LOG_H
