#include "test.h"
#include "utility/guard.h"
#include "locker/locker.h"

USING_NAMESPACE_GOOFER

void test_guard(void)
{
    ThreadLocker thread_locker;
    {
        Guard<ThreadLocker> guard(thread_locker);
    }

    ProcessLocker process_locker;
    {
        Guard<ProcessLocker> guard(process_locker);
    }
}
