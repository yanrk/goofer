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

#ifndef GOOFER_LOG_H
#define GOOFER_LOG_H


#include <cstdarg>
#include <string>
#include "common/common.h"
#include "log/log_types.h"

GOOFER_CXX_API(bool) goofer_log_load_config(const std::string & file, GOOFER_LOG_CONFIG & log_config);
GOOFER_CXX_API(bool) goofer_log_init(const GOOFER_LOG_CONFIG & log_config);
GOOFER_CXX_API(void) goofer_log_exit();
GOOFER_CXX_API(void) goofer_log_enable(GOOFER_LOG_TYPE log_type);
GOOFER_CXX_API(void) goofer_log_disable(GOOFER_LOG_TYPE log_type);
GOOFER_CXX_API(void) goofer_log_enable_all();
GOOFER_CXX_API(void) goofer_log_disable_all();
GOOFER_CXX_API(void) goofer_log_set_level(GOOFER_LOG_TYPE log_type, GOOFER_LOG_LEVEL log_level);
GOOFER_CXX_API(void) goofer_log_set_console(GOOFER_LOG_TYPE log_type, bool output_to_console);
GOOFER_CXX_API(void) goofer_run_log(GOOFER_LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, ...);
GOOFER_CXX_API(void) goofer_debug_log(GOOFER_LOG_LEVEL level, const char * file, const char * func, size_t line, const char * format, ...);

#define RUN_LOG(level, fmt, ...)                                            \
goofer_run_log((level), __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define RUN_LOG_CRI(fmt, ...) RUN_LOG(CRI_LEVEL, (fmt), ##__VA_ARGS__)
#define RUN_LOG_ERR(fmt, ...) RUN_LOG(ERR_LEVEL, (fmt), ##__VA_ARGS__)
#define RUN_LOG_WAR(fmt, ...) RUN_LOG(WAR_LEVEL, (fmt), ##__VA_ARGS__)
#define RUN_LOG_DBG(fmt, ...) RUN_LOG(DBG_LEVEL, (fmt), ##__VA_ARGS__)
#define RUN_LOG_TRK(fmt, ...) RUN_LOG(TRK_LEVEL, (fmt), ##__VA_ARGS__)

#define DBG_LOG(fmt, ...)                                                       \
goofer_debug_log(DBG_LEVEL, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)


#endif // GOOFER_LOG_H
