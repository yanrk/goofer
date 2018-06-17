#include "test.h"
#include "exception/exception.h"

USING_NAMESPACE_GOOFER

void test_exception(void)
{
    try
    {
        GOOFER_THROW("test way 1");
    }
    catch (Exception & e)
    {
        printf("%s\n", e.what().c_str());
    }

    try
    {
        GOOFER_THROW("%s", "test way 2");
    }
    catch (Exception & e)
    {
        printf("%s\n", e.what().c_str());
    }
}
