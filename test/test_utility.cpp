#include "test.h"
#include "utility/utility.h"

USING_NAMESPACE_GOOFER

void test_utility(void)
{
    printf("pid: %u, tid: %u\n", get_pid(), get_tid());
}
