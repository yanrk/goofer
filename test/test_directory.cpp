#include "test.h"
#include "filesystem/directory.h"

USING_NAMESPACE_GOOFER

static void test_directory_1(void)
{
    Directory dir;

    if (!dir.open("."))
    {
        return;
    }

    while (dir.read())
    {
        printf("%s%s\n", dir.sub_path_name().c_str(), (dir.sub_path_is_directory() ? " <dir>" : ""));
    }

    dir.close();
}

static void test_directory_2(void)
{
    std::string dir_name("a/b/c/d");

    goofer_create_directory_recursive(dir_name);

    std::string file;
    std::ofstream ofs;

    file = "a/b/c/d/t.txt";
    ofs.open(file.c_str(), std::ios::trunc | std::ios::binary);
    if (!ofs.is_open())
    {
        std::cout << "create [" << file << "] failed" << std::endl;
    }
    ofs.close();

    file = "a/b/c/t.txt";
    ofs.open(file.c_str(), std::ios::trunc | std::ios::binary);
    if (!ofs.is_open())
    {
        std::cout << "create [" << file << "] failed" << std::endl;
    }
    ofs.close();

    file = "a/b/t.txt";
    ofs.open(file.c_str(), std::ios::trunc | std::ios::binary);
    if (!ofs.is_open())
    {
        std::cout << "create [" << file << "] failed" << std::endl;
    }
    ofs.close();

    file = "a/t.txt";
    ofs.open(file.c_str(), std::ios::trunc | std::ios::binary);
    if (!ofs.is_open())
    {
        std::cout << "create [" << file << "] failed" << std::endl;
    }
    ofs.close();

    goofer_remove_directory_recursive("a");

    file = "a/t.txt";
    ofs.open(file.c_str(), std::ios::trunc | std::ios::binary);
    if (ofs.is_open())
    {
        std::cout << "remove directory failed" << std::endl;
    }
    ofs.close();
}

static void test_directory_3(void)
{
    std::string old_dirname;

    if (!goofer_get_current_work_directory(old_dirname))
    {
        std::cout << "get work directory failed" << std::endl;
        return;
    }

    std::cout << "current work directory: \"" << old_dirname << "\"" << std::endl;

    std::string new_dirname("..");

    if (!goofer_set_current_work_directory(new_dirname))
    {
        std::cout << "set work directory to \"" << new_dirname << "\" failed" << std::endl;
        return;
    }

    std::cout << "set work directory to \"" << new_dirname << "\" success" << std::endl;

    if (!goofer_get_current_work_directory(new_dirname))
    {
        std::cout << "get work directory failed" << std::endl;
        return;
    }

    std::cout << "current work directory: \"" << new_dirname << "\"" << std::endl;

    if (!goofer_set_current_work_directory(old_dirname))
    {
        std::cout << "set work directory to \"" << old_dirname << "\" failed" << std::endl;
        return;
    }

    std::cout << "set work directory to \"" << old_dirname << "\" success" << std::endl;
}

static void test_directory_4(void)
{
    std::cout << "format pathname strictly" << std::endl;

    {
        const std::string src_pathname("c:/abc\\bcd");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    {
        const std::string src_pathname("c:////abc\\///bcd");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    {
        const std::string src_pathname("abc\\bcd");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    {
        const std::string src_pathname("/abc\\bcd");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    {
        const std::string src_pathname("\\\\\\\\abc\\bcd");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    {
        const std::string src_pathname("\\192.168.0.49\\abc\\bcd");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    {
        const std::string src_pathname("\\\\192.168.0.49\\///abc\\\\bcd");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    {
        const std::string src_pathname("\\\\192.168.0.49\\///abc\\\\bcd");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    {
        const std::string src_pathname("\\\\\\192.168.0.49\\///abc\\\\bcd\\\\");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    {
        const std::string src_pathname("www.munu.com:9915\\sys//node_list?id=123456789");
        std::string dst_pathname;
        goofer_pathname_format_strictly(src_pathname, dst_pathname);
        std::cout << "src: " << src_pathname << std::endl;
        std::cout << "dst: " << dst_pathname << std::endl;
    }

    std::cout << std::endl;
}

void test_directory(void)
{
    test_directory_1();
    test_directory_2();
    test_directory_3();
    test_directory_4();
}
