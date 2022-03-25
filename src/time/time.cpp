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

#ifdef _MSC_VER
    #include <windows.h>
    #include <sys/timeb.h>
#endif // _MSC_VER

#include <sstream>
#include <iomanip>
#include "string/string.h"
#include "time/time.h"

NAMESPACE_GOOFER_BEGIN

uint64_t goofer_time()
{
    return (static_cast<uint64_t>(time(nullptr)));
}

uint64_t goofer_ns_time()
{
#ifdef _MSC_VER
    static bool s_got_clock_frequency = false;
    static LARGE_INTEGER s_clock_frequency;
    if (!s_got_clock_frequency)
    {
        QueryPerformanceFrequency(&s_clock_frequency);
        s_got_clock_frequency = true;
    }
    LARGE_INTEGER clock_count;
    QueryPerformanceCounter(&clock_count);
    double current_time = static_cast<double>(clock_count.QuadPart);
    current_time *= 1000000000.0;
    current_time /= static_cast<double>(s_clock_frequency.QuadPart);
    return (static_cast<uint64_t>(current_time));
#elif defined(_MAC_OS)
    return (static_cast<uint64_t>(clock_gettime_nsec_np(CLOCK_MONOTONIC_RAW)));
#else
    struct timespec ts = { 0x0 };
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (static_cast<uint64_t>(ts.tv_sec) * GOOFER_U64_VAL(1000000000) + static_cast<uint64_t>(ts.tv_nsec));
#endif // _MSC_VER
}

uint64_t goofer_monotonic_time()
{
    return (goofer_make_monotonic_time(goofer_ns_time()));
}

uint64_t goofer_make_monotonic_time(uint64_t monotonic_time_nanosecond)
{
    return (monotonic_time_nanosecond / 1000000000);
}

struct tm goofer_make_localtime(uint64_t time_second)
{
    time_t time_value = static_cast<time_t>(time_second);
    struct tm tm_value;

#ifdef _MSC_VER
    localtime_s(&tm_value, &time_value);
#else
    localtime_r(&time_value, &tm_value);
#endif // _MSC_VER

    return (tm_value);
}

struct tm goofer_make_gmtime(uint64_t time_second)
{
    time_t time_value = static_cast<time_t>(time_second);
    struct tm tm_value;

#ifdef _MSC_VER
    gmtime_s(&tm_value, &time_value);
#else
    gmtime_r(&time_value, &tm_value);
#endif // _MSC_VER

    return (tm_value);
}

uint64_t goofer_make_time(struct tm tm_value)
{
    return (static_cast<uint64_t>(mktime(&tm_value)));
}

struct tm goofer_localtime()
{
    return (goofer_make_localtime(goofer_time()));
}

struct tm goofer_gmtime()
{
    return (goofer_make_gmtime(goofer_time()));
}

struct timeval goofer_gettimeofday()
{
    struct timeval tv_now;

#ifdef _MSC_VER
    timeb time_now;
    ftime(&time_now);
    tv_now.tv_sec = static_cast<long>(time_now.time);
    tv_now.tv_usec = static_cast<long>(time_now.millitm) * 1000L;
#else
    gettimeofday(&tv_now, nullptr);
#endif // _MSC_VER

    return (tv_now);
}

int goofer_get_timezone()
{
    long time_diff = 0;

#ifdef _MSC_VER
    _get_timezone(&time_diff);
#else
    time_diff = timezone;
#endif // _MSC_VER

    time_diff /= -3600;

    return (time_diff);
}

int goofer_get_day_of_week(const struct tm & tm_value)
{
    return (tm_value.tm_wday);
}

int goofer_get_day_of_week()
{
    return (goofer_get_day_of_week(goofer_localtime()));
}

std::string goofer_get_week(const struct tm & tm_value, bool week_abbreviation)
{
    const char * const week[][7] = 
    {
        { "Sun",    "Mon",    "Tue",     "Wed",       "Thu",      "Fri",    "Sat"      }, 
        { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" }  
    };
    return (std::string(week[week_abbreviation ? 0 : 1][goofer_get_day_of_week(tm_value)]));
}

std::string goofer_get_week(bool week_abbreviation)
{
    return (goofer_get_week(goofer_localtime(), week_abbreviation));
}

std::string goofer_get_date(const struct tm & tm_value, const char * date_delimiter)
{
    if (nullptr == date_delimiter)
    {
        date_delimiter = "-";
    }

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(4) << (tm_value.tm_year + 1900)
        << date_delimiter
        << std::setw(2) << (tm_value.tm_mon + 1)
        << date_delimiter
        << std::setw(2) << (tm_value.tm_mday);
    return (oss.str());
}

std::string goofer_get_date(const char * date_delimiter)
{
    return (goofer_get_date(goofer_localtime(), date_delimiter));
}

std::string goofer_get_time(const struct tm & tm_value, const char * time_delimiter)
{
    if (nullptr == time_delimiter)
    {
        time_delimiter = ":";
    }

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << (tm_value.tm_hour)
        << time_delimiter
        << std::setw(2) << (tm_value.tm_min)
        << time_delimiter
        << std::setw(2) << (tm_value.tm_sec);
    return (oss.str());
}

std::string goofer_get_time(const char * time_delimiter)
{
    return (goofer_get_time(goofer_localtime(), time_delimiter));
}

std::string goofer_datetime()
{
    time_t t_now = time(nullptr);
    struct tm tm_now = { 0x0 };

#ifdef _MSC_VER
    localtime_s(&tm_now, &t_now);
#else
    localtime_r(&t_now, &tm_now);
#endif // _MSC_VER

    char str_time[32] = { 0x0 };
    strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M:%S", &tm_now);

    return (str_time);
}

std::string goofer_get_datetime(const struct tm & tm_value, const char * date_delimiter, const char * time_delimiter, const char * date_time_delimiter)
{
    if (nullptr == date_time_delimiter)
    {
        date_time_delimiter = " ";
    }
    return (goofer_get_date(tm_value, date_delimiter) + date_time_delimiter + goofer_get_time(tm_value, time_delimiter));
}

std::string goofer_get_datetime(const char * date_delimiter, const char * time_delimiter, const char * date_time_delimiter)
{
    return (goofer_get_datetime(goofer_localtime(), date_delimiter, time_delimiter, date_time_delimiter));
}

std::string goofer_get_comprehensive_datetime(
                const char * date_delimiter, 
                const char * time_delimiter, 
                const char * date_time_delimiter, 
                bool week_abbreviation
            )
{
    struct timeval tv_now = goofer_gettimeofday();
    size_t time_ms = static_cast<size_t>(tv_now.tv_usec / 1000);
    uint64_t time_now = static_cast<uint64_t>(tv_now.tv_sec);
    struct tm tm_now = goofer_make_localtime(time_now);

    char buff[32] = { 0 };
    goofer_snprintf
    (
        buff, sizeof(buff), ".%03u GMT%+03d:00 ", 
        time_ms, goofer_get_timezone()
    );

    return (goofer_get_datetime(tm_now, date_delimiter, time_delimiter, date_time_delimiter) + buff + goofer_get_week(tm_now));
}

void goofer_ms_sleep(size_t milliseconds)
{
    uint64_t nanoseconds = GOOFER_U64_VAL(1000000) * milliseconds;
    goofer_ns_sleep(nanoseconds);
}

void goofer_ns_sleep(uint64_t nanoseconds)
{
#ifdef _MSC_VER
    Sleep(static_cast<DWORD>(nanoseconds / 1000000));
#else
    struct timespec sleep_time;
    sleep_time.tv_sec = static_cast<time_t>(nanoseconds / 1000000000);
    sleep_time.tv_nsec = static_cast<long>(nanoseconds % 1000000000);

    while (0 != nanosleep(&sleep_time, &sleep_time) && EINTR == errno)
    {
        /* continue */
    }
#endif // _MSC_VER
}

NAMESPACE_GOOFER_END
