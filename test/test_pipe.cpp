#include "test.h"
#include "filesystem/directory.h"
#include "filesystem/file.h"
#include "charset/charset.h"
#include "process/process.h"
#include "pipe/pipe.h"
#include <string>
#include <sstream>

USING_NAMESPACE_GOOFER

void test_read_anonymous_pipe(void)
{
#ifdef _MSC_VER
    const std::string command_line("ping -n 10 127.0.0.1");
#else
    const std::string command_line("/bin/ping -c 10 127.0.0.1");
#endif // _MSC_VER

    AnonymousPipe r_pipe;
    if (!r_pipe.acquire(true, false))
    {
        printf("create read anonymous pipe failed\n");
        return;
    }

    size_t parent_reader = 0;
    size_t parent_writer = 0;
    size_t child_reader = 0;
    size_t child_writer = 0;
    r_pipe.get(parent_reader, parent_writer, child_reader, child_writer);

    Process r_process;
    r_process.set_process_args(command_line);

    if (!r_process.acquire(parent_reader, parent_writer, child_reader, child_writer))
    {
        printf("create read process failed\n");
        return;
    }

    r_pipe.close_child_endpoint();

    printf("anonymous pipe read begin\n");

    while (true)
    {
        char buff[512] = { 0x0 };
        size_t read_len = 0;
        if (r_pipe.read(buff, sizeof(buff), read_len))
        {
            if (read_len > 0)
            {
                printf("%s", buff);
            }
            else
            {
                break;
            }
        }
        else
        {
            printf("anonymous pipe read failure (%d)\n", goofer_system_error());
            return;
        }
    }

    printf("anonymous pipe read end\n");

    r_pipe.close_parent_endpoint();

    int exit_code = 0;
    r_process.wait_exit(exit_code);
    r_process.release();

    r_pipe.release();
}

void test_write_anonymous_pipe(void)
{
#ifdef _MSC_VER
    const std::string program("./hole.exe");
#else
    const std::string program("./hole");
#endif // _MSC_VER

    if (!goofer_access_safe(program.c_str()))
    {
        printf("test write anonymous pipe failed: (%s) not exist\n", program.c_str());
        return;
    }

    AnonymousPipe w_pipe;
    if (!w_pipe.acquire(false, true))
    {
        printf("create write anonymous pipe failed\n");
        return;
    }

    size_t parent_reader = 0;
    size_t parent_writer = 0;
    size_t child_reader = 0;
    size_t child_writer = 0;
    w_pipe.get(parent_reader, parent_writer, child_reader, child_writer);

    Process w_process;
    w_process.set_process_args(program);

    if (!w_process.acquire(parent_reader, parent_writer, child_reader, child_writer))
    {
        printf("create write process failed\n");
        return;
    }

    w_pipe.close_child_endpoint();

    for (int index = 0, count = 5; index < count; ++index)
    {
        std::ostringstream oss;
        oss << "write anonymous pipe test " << (index + 1) << "/" << count << std::endl;
        std::string data = oss.str();
        if (w_pipe.write(data.c_str(), data.size()))
        {
            printf("anonymous pipe write data (%d) success\n", index + 1);
        }
        else
        {
            printf("anonymous pipe write data (%d) failure\n", index + 1);
            return;
        }
    }

    w_pipe.close_parent_endpoint();

    int exit_code = 0;
    w_process.wait_exit(exit_code);
    w_process.release();

    w_pipe.release();
}

void test_read_write_anonymous_pipe(void)
{
#ifdef _MSC_VER
    const std::string program("./echo.exe");
#else
    const std::string program("./echo");
#endif // _MSC_VER

    if (!goofer_access_safe(program.c_str()))
    {
        printf("test read / write anonymous pipe failed: (%s) not exist\n", program.c_str());
        return;
    }

    AnonymousPipe rw_pipe;
    if (!rw_pipe.acquire(true, true))
    {
        printf("create read / write anonymous pipe failed\n");
        return;
    }

    size_t parent_reader = 0;
    size_t parent_writer = 0;
    size_t child_reader = 0;
    size_t child_writer = 0;
    rw_pipe.get(parent_reader, parent_writer, child_reader, child_writer);

    Process rw_process;
    rw_process.set_process_args(program);

    if (!rw_process.acquire(parent_reader, parent_writer, child_reader, child_writer))
    {
        printf("create read / write process failed\n");
        return;
    }

    rw_pipe.close_child_endpoint();

    for (size_t index = 0, count = 5; index < count; ++index)
    {
        std::ostringstream oss;
        oss << "read / write anonymous pipe test " << (index + 1) << "/" << count;
        std::string data = oss.str();
        if (rw_pipe.write(data.c_str(), data.size()))
        {
            printf("anonymous pipe write (%s) success\n", data.c_str());
        }
        else
        {
            printf("anonymous pipe write (%s) failure\n", data.c_str());
            return;
        }
        char buff[64] = { 0x0 };
        size_t read_len = 0;
        if (rw_pipe.read(buff, sizeof(buff), read_len))
        {
            printf("anonymous pipe read success (%s)\n", buff);
        }
        else
        {
            printf("anonymous pipe read failure\n");
            return;
        }
    }

    rw_pipe.close_parent_endpoint();

    int exit_code = 0;
    rw_process.wait_exit(exit_code);
    rw_process.release();

    rw_pipe.release();
}

void test_pipe(void)
{
    std::string pathname;
    goofer_get_current_process_pathname(pathname);
    std::string dirname;
    goofer_extract_directory(pathname.c_str(), dirname);
    goofer_set_current_work_directory(ansi_to_utf8(dirname));

    test_read_anonymous_pipe();
    test_write_anonymous_pipe();
    test_read_write_anonymous_pipe();
}
