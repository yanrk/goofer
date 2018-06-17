#include "test.h"
#include "common/common.h"

USING_NAMESPACE_GOOFER

void test_memory(void)
{
    {
        int * i = nullptr;
        GOOFER_NEW(i, int);
        *i = 5;
        GOOFER_DEL(i);
        assert(nullptr == i);
    }

    {
        int * i = nullptr;
        GOOFER_NEW(i, int(1));
        assert(1 == *i);
        GOOFER_DEL(i);
        assert(nullptr == i);
    }

    {
        int * i = nullptr;
        GOOFER_NEW_ARR(i, int, 1);
        i[0] = 5;
        GOOFER_DEL_ARR(i);
        assert(nullptr == i);
    }

    {
        std::string * s = nullptr;
        GOOFER_NEW(s, std::string);
        *s = "hello world";
        GOOFER_DEL(s);
        assert(nullptr == s);
    }

    {
        std::string * s = nullptr;
        GOOFER_NEW(s, std::string("hello world"));
        assert("hello world" == *s);
        GOOFER_DEL(s);
        assert(nullptr == s);
    }

    {
        std::string * s = nullptr;
        GOOFER_NEW_ARR(s, std::string, 1);
        s[0] = "hello world";
        GOOFER_DEL_ARR(s);
        assert(nullptr == s);
    }
}
