#include "test.h"
#include "mmap/mmap.h"
#include "charset/charset.h"

USING_NAMESPACE_GOOFER

static void test_mem_map()
{
    const std::string map_name = ansi_to_utf8(__FUNCTION__);

    MemMap map_writer;
    if (!map_writer.init(map_name.c_str(), true, 100, false))
    {
        std::cout << "create memory map failed" << std::endl;
        return;
    }

    MemMap map_reader;
    if (!map_reader.init(map_name.c_str(), false, 100, false))
    {
        std::cout << "open memory map failed" << std::endl;
        return;
    }

    {
        char src[64] = "hello, world! this is a memory map test 1";
        memcpy(map_writer.get_map_data(), src, sizeof(src));

        char dst[64] = { 0x0 };
        memcpy(dst, map_reader.get_map_data(), sizeof(dst));

        if (0 != memcmp(src, dst, sizeof(src)))
        {
            std::cout << "memory map test failed" << std::endl;
            return;
        }
    }

    map_writer.exit();

    if (!map_writer.init(map_name.c_str(), true, 100, false))
    {
        std::cout << "create memory map failed" << std::endl;
        return;
    }

    {
        char src[64] = "hello, again! this is a memory map test 2";
        memcpy(map_writer.get_map_data(), src, sizeof(src));

        char dst[64] = { 0x0 };
        memcpy(dst, map_reader.get_map_data(), sizeof(dst));

        if (0 != memcmp(src, dst, sizeof(src)))
        {
            std::cout << "memory map test failed" << std::endl;
            return;
        }
    }

    map_writer.exit();
    map_reader.exit();

    std::cout << "memory map test success" << std::endl;
}

static void test_file_map()
{
    const std::string file_name = ansi_to_utf8(__FILE__) + ".mmap";

    FileMap map_writer;
    if (!map_writer.init(file_name.c_str(), true, 100, false))
    {
        std::cout << "create file map failed" << std::endl;
        return;
    }

    FileMap map_reader;
    if (!map_reader.init(file_name.c_str(), false, 100, false))
    {
        std::cout << "open file map failed" << std::endl;
        return;
    }

    {
        char src[64] = "hello, world! this is a file map test 1";
        memcpy(map_writer.get_map_data(), src, sizeof(src));

        char dst[64] = { 0x0 };
        memcpy(dst, map_reader.get_map_data(), sizeof(dst));

        if (0 != memcmp(src, dst, sizeof(src)))
        {
            std::cout << "file map test failed" << std::endl;
            return;
        }
    }

    map_writer.exit();

    if (!map_writer.init(file_name.c_str(), true, 100, false))
    {
        std::cout << "create file map failed" << std::endl;
        return;
    }

    {
        char src[64] = "hello, again! this is a file map test 2";
        memcpy(map_writer.get_map_data(), src, sizeof(src));

        char dst[64] = { 0x0 };
        memcpy(dst, map_reader.get_map_data(), sizeof(dst));

        if (0 != memcmp(src, dst, sizeof(src)))
        {
            std::cout << "file map test failed" << std::endl;
            return;
        }
    }

    map_writer.exit();
    map_reader.exit();

    std::cout << "file map test success" << std::endl;
}

void test_mmap(void)
{
    test_mem_map();
    test_file_map();
}
