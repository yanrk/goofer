/********************************************************
 * Description : time functions
 * Data        : 2013-05-24 12:15:59
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_TIME_H
#define GOOFER_TIME_H


#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <sys/time.h>
    #include <unistd.h>
    #include <errno.h>
#endif // _MSC_VER

#include <ctime>
#include <cstdint>
#include <string>
#include "common/common.h"

NAMESPACE_GOOFER_BEGIN

GOOFER_CXX_API(uint64_t) goofer_time();
GOOFER_CXX_API(uint64_t) goofer_ns_time();
GOOFER_CXX_API(uint64_t) goofer_monotonic_time();
GOOFER_CXX_API(uint64_t) goofer_make_monotonic_time(uint64_t monotonic_time_nanosecond);
GOOFER_CXX_API(struct tm) goofer_localtime();
GOOFER_CXX_API(struct tm) goofer_gmtime();
GOOFER_CXX_API(struct tm) goofer_make_localtime(uint64_t time_second);
GOOFER_CXX_API(struct tm) goofer_make_gmtime(uint64_t time_second);
GOOFER_CXX_API(uint64_t) goofer_make_time(struct tm tm_value);
GOOFER_CXX_API(struct timeval) goofer_gettimeofday();
GOOFER_CXX_API(int) goofer_get_timezone();
GOOFER_CXX_API(int) goofer_get_day_of_week();
GOOFER_CXX_API(int) goofer_get_day_of_week(const struct tm & tm_value);
GOOFER_CXX_API(std::string) goofer_get_week(bool week_abbreviation = true);
GOOFER_CXX_API(std::string) goofer_get_week(const struct tm & tm_value, bool week_abbreviation = true);
GOOFER_CXX_API(std::string) goofer_get_date(const char * date_delimiter = "-");
GOOFER_CXX_API(std::string) goofer_get_date(const struct tm & tm_value, const char * date_delimiter = "-");
GOOFER_CXX_API(std::string) goofer_get_time(const char * time_delimiter = ":");
GOOFER_CXX_API(std::string) goofer_get_time(const struct tm & tm_value, const char * time_delimiter = ":");
GOOFER_CXX_API(std::string) goofer_datetime();
GOOFER_CXX_API(std::string) goofer_get_datetime(const char * date_delimiter = "-", const char * time_delimiter = ":", const char * date_time_delimiter = " ");
GOOFER_CXX_API(std::string) goofer_get_datetime(const struct tm & tm_value, const char * date_delimiter = "-", const char * time_delimiter = ":", const char * date_time_delimiter = " ");
GOOFER_CXX_API(std::string) goofer_get_comprehensive_datetime(const char * date_delimiter = "-", const char * time_delimiter = ":", const char * date_time_delimiter = " ", bool week_abbreviation = true);
GOOFER_CXX_API(void) goofer_ms_sleep(size_t milliseconds);
GOOFER_CXX_API(void) goofer_ns_sleep(uint64_t nanoseconds);

NAMESPACE_GOOFER_END


#endif // GOOFER_TIME_H
