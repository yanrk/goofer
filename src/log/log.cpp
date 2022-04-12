/********************************************************
 * Description : interface of log module
 * Date        : 2013-05-22 22:34:24
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cassert>
#include <cstring>
#include <string>
#include "config/ini.h"
#include "log/log_types.h"
#include "log/log_base.h"
#include "log/lazy_log.h"
#include "log/sync_log.h"
#include "log/asyn_log.h"
#include "log/log.h"

USING_NAMESPACE_GOOFER

static bool transform_log_write_mode(const std::string & key_value, GOOFER_LOG_WRITE_MODE & write_mode)
{
    if ("SYNC_WRITE_MODE" == key_value)
    {
        write_mode = SYNC_WRITE_MODE;
    }
    else if ("ASYN_WRITE_MODE" == key_value)
    {
        write_mode = ASYN_WRITE_MODE;
    }
    else if ("LAZY_WRITE_MODE" == key_value)
    {
        write_mode = LAZY_WRITE_MODE;
    }
    else
    {
        return (false);
    }

    return (true);
}

static bool transform_log_level(const std::string & key_value, GOOFER_LOG_LEVEL & log_level)
{
    if ("CRI_LEVEL" == key_value)
    {
        log_level = CRI_LEVEL;
    }
    else if ("ERR_LEVEL" == key_value)
    {
        log_level = ERR_LEVEL;
    }
    else if ("WAR_LEVEL" == key_value)
    {
        log_level = WAR_LEVEL;
    }
    else if ("DBG_LEVEL" == key_value)
    {
        log_level = DBG_LEVEL;
    }
    else if ("TRK_LEVEL" == key_value)
    {
        log_level = TRK_LEVEL;
    }
    else
    {
        return (false);
    }

    return (true);
}

bool goofer_log_load_config(const std::string & file, GOOFER_LOG_CONFIG & log_config)
{
    Ini log_ini;
    if (!log_ini.load(file))
    {
        return (false);
    }

    if (!log_ini.get_value("", "log_path", log_config.log_file_path))
    {
        return (false);
    }

    for (int type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
    {
        std::string app_name(GOOFER_LOG_TYPE_INFO[type]);
        std::string key_value;

        if (!log_ini.get_value(app_name, "write_mode", key_value))
        {
            return (false);
        }

        if (!transform_log_write_mode(key_value, log_config.log_file[type].write_mode))
        {
            return (false);
        }

        if (!log_ini.get_value(app_name, "min_level", key_value))
        {
            return (false);
        }

        if (!transform_log_level(key_value, log_config.log_file[type].log_min_level))
        {
            return (false);
        }

        if (!log_ini.get_value(app_name, "file_size", log_config.log_file[type].log_file_size))
        {
            return (false);
        }

        if (!log_ini.get_value(app_name, "buffer_count", log_config.log_file[type].buffer_count))
        {
            return (false);
        }

        if (!log_ini.get_value(app_name, "output_to_console", log_config.log_file[type].output_to_console))
        {
            return (false);
        }
    }

    return (true);
}

static LogBase * s_log[LOG_TYPE_MAX] = { nullptr };
static unsigned char s_log_type_open[LOG_TYPE_MAX] = { 0x0 };
static bool s_need_aysn_write_thread = false;

bool goofer_log_init(const GOOFER_LOG_CONFIG & log_config)
{
    for (size_t type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
    {
        switch (log_config.log_file[type].write_mode)
        {
            case SYNC_WRITE_MODE:
            {
                GOOFER_NEW
                (
                    s_log[type], 
                    SyncLog
                    (
                        log_config.log_file_path, 
                        GOOFER_LOG_TYPE_INFO[type], 
                        log_config.log_file[type].log_min_level, 
                        log_config.log_file[type].log_file_size, 
                        log_config.log_file[type].output_to_console
                    )
                );
                break;
            }
            case ASYN_WRITE_MODE:
            {
                GOOFER_NEW
                (
                    s_log[type], 
                    AsynLog
                    (
                        log_config.log_file_path, 
                        GOOFER_LOG_TYPE_INFO[type], 
                        log_config.log_file[type].log_min_level, 
                        log_config.log_file[type].log_file_size, 
                        log_config.log_file[type].output_to_console
                    )
                );
                s_need_aysn_write_thread = true;
                break;
            }
            case LAZY_WRITE_MODE:
            {
                GOOFER_NEW
                (
                    s_log[type], 
                    LazyLog
                    (
                        log_config.log_file_path, 
                        GOOFER_LOG_TYPE_INFO[type], 
                        log_config.log_file[type].log_min_level, 
                        log_config.log_file[type].log_file_size, 
                        log_config.log_file[type].buffer_count, 
                        log_config.log_file[type].output_to_console
                    )
                );
                break;
            }
            default:
            {
                assert(false);
                return (false);
            }
        }

        if (nullptr == s_log[type])
        {
            assert(nullptr != s_log[type]);
            return (false);
        }
    }

    goofer_log_enable_all();

    if (s_need_aysn_write_thread)
    {
        return (AsynLog::acquire_write_thread());
    }

    return (true);
}

void goofer_log_exit()
{
    goofer_log_disable_all();

    if (s_need_aysn_write_thread)
    {
        AsynLog::release_write_thread();
        s_need_aysn_write_thread = false;
    }

    for (size_t type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
    {
        GOOFER_DEL(s_log[type]);
    }
}

void goofer_log_enable(GOOFER_LOG_TYPE log_type)
{
    s_log_type_open[log_type] = 0x1;
}

void goofer_log_disable(GOOFER_LOG_TYPE log_type)
{
    s_log_type_open[log_type] = 0x0;
}

void goofer_log_enable_all()
{
    memset(s_log_type_open, 0x01, sizeof(s_log_type_open));
}

void goofer_log_disable_all()
{
    memset(s_log_type_open, 0x0, sizeof(s_log_type_open));
}

void goofer_log_set_level(GOOFER_LOG_TYPE log_type, GOOFER_LOG_LEVEL log_level)
{
    if (log_type < LOG_TYPE_MIN || log_type >= LOG_TYPE_MAX)
    {
        return;
    }

    if (nullptr != s_log[log_type])
    {
        s_log[log_type]->set_min_level(log_level);
    }
}

void goofer_log_set_console(GOOFER_LOG_TYPE log_type, bool output_to_console)
{
    if (log_type < LOG_TYPE_MIN || log_type >= LOG_TYPE_MAX)
    {
        return;
    }

    if (nullptr != s_log[log_type])
    {
        s_log[log_type]->set_console_output_switch(output_to_console);
    }
}

void goofer_run_log(GOOFER_LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, ...)
{
    {
        va_list args;

        va_start(args, format);

        if (0x0 != s_log_type_open[LOG_TYPE_RUN] && nullptr != s_log[LOG_TYPE_RUN])
        {
            s_log[LOG_TYPE_RUN]->push_record(level, file, func, line, format, args);
        }

        va_end(args);
    }

    {
        va_list args;

        va_start(args, format);

        if (0x0 != s_log_type_open[LOG_TYPE_DBG] && nullptr != s_log[LOG_TYPE_DBG])
        {
            s_log[LOG_TYPE_DBG]->push_record(level, file, func, line, format, args);
        }

        va_end(args);
    }
}

void goofer_debug_log(GOOFER_LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, ...)
{
    if (0x0 == s_log_type_open[LOG_TYPE_DBG] || nullptr == s_log[LOG_TYPE_DBG])
    {
        return;
    }

    va_list args;

    va_start(args, format);

    s_log[LOG_TYPE_DBG]->push_record(level, file, func, line, format, args);

    va_end(args);
}
