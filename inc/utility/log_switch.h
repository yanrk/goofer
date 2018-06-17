/********************************************************
 * Description : log switch (assistant of log module)
 * Data        : 2013-10-19 10:56:24
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_LOG_SWITCH_H
#define GOOFER_LOG_SWITCH_H


#include "common/common.h"
#include "utility/uncopy.h"
#include "utility/singleton.h"
#include "log/log_types.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API LogSwitch : private Uncopy
{
public:
    bool init(const char * config_file = nullptr);
    bool init(const GOOFER_LOG_CONFIG & log_config);
    void exit();

public:
    void enable(GOOFER_LOG_TYPE log_type);
    void disable(GOOFER_LOG_TYPE log_type);
    void enable_all();
    void disable_all();
    void set_min_level(GOOFER_LOG_TYPE log_type, GOOFER_LOG_LEVEL log_level);
    void set_console_output_switch(GOOFER_LOG_TYPE log_type, bool output_to_console);

private:
    LogSwitch();
    ~LogSwitch();

private:
    friend class Singleton<LogSwitch>;

private:
    bool                   m_init;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_LOG_SWITCH_H
