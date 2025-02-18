/********************************************************
 * Description : log switch (assistant of log module)
 * Date        : 2013-10-19 10:56:24
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include "log/log.h"
#include "utility/log_switch.h"

NAMESPACE_GOOFER_BEGIN

LogSwitch::LogSwitch()
    : m_init(false)
{

}

LogSwitch::~LogSwitch()
{
    exit();
}

bool LogSwitch::init(const char * config_file)
{
    if (nullptr == config_file || '\0' == config_file[0])
    {
        config_file = "../../config/log.ini";
    }

    GOOFER_LOG_CONFIG log_config;
    const std::string config_name(config_file);

    if (!goofer_log_load_config(config_name, log_config))
    {
        log_config.log_file_path = "./log/";
        for (size_t type = LOG_TYPE_MIN; type < LOG_TYPE_MAX; ++type)
        {
            GOOFER_LOG_CONFIG::FILE_CONFIG & file_config = log_config.log_file[type];
            file_config.write_mode = SYNC_WRITE_MODE;
            file_config.log_min_level = DBG_LEVEL;
            file_config.log_file_size = 10;
            file_config.buffer_count = 50;
            file_config.output_to_console = false;
            file_config.auto_delete_file = false;
            file_config.max_keep_count = 50;
            file_config.max_keep_days = 7;
        }
    }

    return (init(log_config));
}

bool LogSwitch::init(const GOOFER_LOG_CONFIG & log_config)
{
    if (m_init)
    {
        return (true);
    }

    m_init = goofer_log_init(log_config);

    return (m_init);
}

void LogSwitch::exit()
{
    if (!m_init)
    {
        return;
    }

    goofer_log_exit();

    m_init = false;
}

void LogSwitch::enable(GOOFER_LOG_TYPE log_type)
{
    goofer_log_enable(log_type);
}

void LogSwitch::disable(GOOFER_LOG_TYPE log_type)
{
    goofer_log_disable(log_type);
}

void LogSwitch::enable_all()
{
    goofer_log_enable_all();
}

void LogSwitch::disable_all()
{
    goofer_log_disable_all();
}

void LogSwitch::set_min_level(GOOFER_LOG_TYPE log_type, GOOFER_LOG_LEVEL log_level)
{
    goofer_log_set_level(log_type, log_level);
}

void LogSwitch::set_console_output_switch(GOOFER_LOG_TYPE log_type, bool output_to_console)
{
    goofer_log_set_console(log_type, output_to_console);
}

NAMESPACE_GOOFER_END
