#include "test.h"
#include "utility/counter.h"

USING_NAMESPACE_GOOFER

void test_counter(void)
{
    Counter counter(5);
    if (counter.valid())
    {
        std::cout << "counter references is " << counter.references() << std::endl;
        counter.increase();
        std::cout << "increase once, counter references is " << counter.references() << std::endl;
        counter.decrease();
        std::cout << "decrease once, counter references is " << counter.references() << std::endl;
    }
    else
    {
        std::cout << "counter is invalid" << std::endl;
    }
}
