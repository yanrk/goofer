#include "test.h"
#include "time/time.h"

USING_NAMESPACE_GOOFER

void test_time(void)
{
    size_t time_now = static_cast<size_t>(goofer_time());
    printf("seconds from 1900-01-01 00:00:00 is %u\n", time_now);

    tm tm_utc_now = goofer_gmtime();
    printf
    (
        "UTC: %4d-%02d-%02d %02d:%02d:%02d\n", 
        tm_utc_now.tm_year + 1900, tm_utc_now.tm_mon + 1, tm_utc_now.tm_mday, 
        tm_utc_now.tm_hour, tm_utc_now.tm_min, tm_utc_now.tm_sec
    );

    static const char * const week[7] = 
    {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    tm tm_now = goofer_localtime();
    struct timeval tv_now = goofer_gettimeofday();
    int millisecond = static_cast<int>(tv_now.tv_usec / 1000);
    int time_zone = goofer_get_timezone() / -3600;
    int day_of_week = goofer_get_day_of_week();

    printf
    (
        "Local Time: %4d-%02d-%02d %02d:%02d:%02d.%03d GMT%+03d:00 %s\n", 
        tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday, 
        tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec, millisecond, 
        time_zone, week[day_of_week]
    );

    goofer_ms_sleep(1000);

    int64_t t_start = goofer_time();
    goofer_ns_sleep(GOOFER_I64_VAL(5000000000));
    int64_t t_end = goofer_time();

    int sleep_seconds = static_cast<int>(t_end - t_start);
    if (sleep_seconds < 5 - 1 || sleep_seconds > 5 + 1)
    {
        printf("goofer_ns_sleep maybe not accurate\n");
    }
    else
    {
        printf("goofer_ns_sleep works well\n");
    }

    printf("comprehensive date-time: %s\n", goofer_get_comprehensive_datetime().c_str());
    printf("date-time: %s\n", goofer_get_datetime().c_str());
    printf("date: %s\n", goofer_get_date().c_str());
    printf("time: %s\n", goofer_get_time().c_str());
    printf("week: %s\n", goofer_get_week().c_str());
}
