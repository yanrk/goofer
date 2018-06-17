#include "test.h"
#include "time/time.h"
#include "time/single_timer.h"

USING_NAMESPACE_GOOFER

class TimerSink : public ISingleTimerSink
{
public:
    TimerSink();

public:
    virtual void on_timer(bool first_time, size_t index);

private:
    size_t        m_count;
    SingleTimer   m_timer;
};

TimerSink::TimerSink()
    : m_count(0)
    , m_timer()
{
    m_timer.init(this, 100);
}

void TimerSink::on_timer(bool first_time, size_t index)
{
    if (first_time)
    {
        std::cout << "...on timer..." << std::endl;
    }
    std::cout << "index: " << index << " --- "
              << "count: " << m_count++ << std::endl;
}

void test_single_timer(void)
{
    std::cout << "sleep 1000ms begin..." << std::endl;
    {
        TimerSink sink;
        goofer_ms_sleep(1000);
    }
    std::cout << "sleep 1000ms end..." << std::endl;
}
