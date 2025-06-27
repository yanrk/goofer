#include "test.h"
#include "utility/utility.h"

USING_NAMESPACE_GOOFER

void test_utility(void)
{
    printf("pid: %u, tid: " GOOFER_U64_FMT "\n", get_pid(), get_tid());
}
