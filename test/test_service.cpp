#include "test.h"
#include "filesystem/service.h"
#include "filesystem/directory.h"
#include "thread/thread.h"
#include "time/time.h"

USING_NAMESPACE_GOOFER

class TestService : public SystemServiceBase
{
public:
    TestService();
    virtual ~TestService();

public:
    virtual bool on_start(int argc, char * argv[]) override;
    virtual bool on_stop() override;
    virtual bool running() override;

public:
    void record_data();

private:
    bool            m_running;
    std::ofstream   m_fstream;
    Thread          m_record_thread;
};

static thread_return_t GOOFER_STDCALL record_thread(thread_argument_t argument)
{
    TestService * test_service = reinterpret_cast<TestService *>(argument);
    while (test_service->running())
    {
        test_service->record_data();
        goofer_ms_sleep(1000 * 5);
    }
    return (THREAD_DEFAULT_RET);
}

TestService::TestService()
    : m_running(false)
    , m_fstream()
    , m_record_thread()
{
    m_record_thread.set_thread_args(record_thread, this);
}

TestService::~TestService()
{
    on_stop();
}

void TestService::record_data()
{
    if (m_fstream.is_open())
    {
        m_fstream << goofer_get_comprehensive_datetime() << std::endl;
    }
}

bool TestService::on_start(int argc, char * argv[])
{
    if (argc <= 0 || nullptr == argv)
    {
        return (false);
    }

    const std::string test_file(goofer_extract_directory(argv[0], false) + "/test_service.log");
    m_fstream.open(test_file.c_str(), std::ios::app | std::ios::binary);
    if (!m_fstream.is_open())
    {
        return (false);
    }

    m_running = true;

    if (!m_record_thread.acquire())
    {
        m_running = false;
        return (false);
    }

    return (true);
}

bool TestService::on_stop()
{
    m_running = false;
    m_record_thread.release();
    m_fstream.close();
    return (true);
}

bool TestService::running()
{
    return (m_running);
}

void test_service(int argc, char * argv[])
{
    TestService test_service;
    test_service.run("test_service", argc, argv);
}
