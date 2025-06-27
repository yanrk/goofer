#include "test.h"
#include "utility/utility.h"

USING_NAMESPACE_GOOFER

void test_utility(void)
{
    printf("pid: %u, tid: %zu\n", get_pid(), get_tid());
}
