#include "test.h"
#include "utility/uncopy.h"
#include "utility/singleton.h"

USING_NAMESPACE_GOOFER

class identifier : private Uncopy
{
public:
    void set(const std::string & id)
    {
        m_id = id;
    }

    const std::string & get() const
    {
        return (m_id);
    }

private:
    identifier()
    {

    }

    ~identifier()
    {

    }

    friend class Singleton<identifier>;

private:
    std::string      m_id;
};

void test_singleton(void)
{
    Singleton<identifier>::instance().set("364321198712154135");
    std::cout << "identifier: " << Singleton<identifier>::instance().get() << std::endl;
}
