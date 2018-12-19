#include "test.h"
#include "filesystem/directory.h"
#include "filesystem/file.h"
#include "charset/charset.h"
#include "process/process.h"
#include "thread/thread.h"
#include "locker/locker.h"
#include "pipe/pipe.h"
#include <string>
#include <sstream>

USING_NAMESPACE_GOOFER

static ThreadLocker   s_stdout_locker;

static void test_read_anonymous_pipe(void)
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

static void test_write_anonymous_pipe(void)
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

static void test_read_write_anonymous_pipe(void)
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

static void test_anonymous_pipe()
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

static thread_return_t GOOFER_STDCALL test_read_named_pipe_client(thread_argument_t)
{
    NamedPipe r_pipe;

    if (!r_pipe.acquire("name_r", false, false, true))
    {
        s_stdout_locker.acquire();
        std::cout << "[client] create named pipe for read failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }

    char normal_data[] = "read pipe test data";
    for (std::size_t index = 0; index < 5; ++index)
    {
        if (!r_pipe.write(normal_data, sizeof(normal_data) - 1))
        {
            s_stdout_locker.acquire();
            std::cout << "[client] named pipe write failed" << std::endl;
            s_stdout_locker.release();
            return (THREAD_DEFAULT_RET);
        }
        if (!r_pipe.write("\n", 1))
        {
            s_stdout_locker.acquire();
            std::cout << "[client] named pipe write failed" << std::endl;
            s_stdout_locker.release();
            return (THREAD_DEFAULT_RET);
        }
        s_stdout_locker.acquire();
        std::cout << "[client]     pipe_r write data [" << normal_data << "]" << std::endl;
        s_stdout_locker.release();
    }

    char quit_data[] = "exit";
    if (!r_pipe.write(quit_data, sizeof(quit_data) - 1))
    {
        s_stdout_locker.acquire();
        std::cout << "[client] named pipe write failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }
    if (!r_pipe.write("\n", 1))
    {
        s_stdout_locker.acquire();
        std::cout << "[client] named pipe write failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }

    s_stdout_locker.acquire();
    std::cout << "[client]     pipe_r write data [" << quit_data << "]" << std::endl;
    s_stdout_locker.release();

    r_pipe.release();

    return (THREAD_DEFAULT_RET);
}

static thread_return_t GOOFER_STDCALL test_read_named_pipe_server(thread_argument_t)
{
    NamedPipe r_pipe;

    if (!r_pipe.acquire("name_r", true, true, false))
    {
        s_stdout_locker.acquire();
        std::cout << "[server] create named pipe for read failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }

    char buff[64] = { 0x0 };
    const std::size_t buff_size = sizeof(buff) - 1;

    while (true)
    {
        std::size_t read_data_len = 0;
        while (read_data_len < buff_size)
        {
            std::size_t read_len = 0;
            if (!r_pipe.read(buff + read_data_len, 1, read_len))
            {
                s_stdout_locker.acquire();
                std::cout << "[server] named pipe read failed" << std::endl;
                s_stdout_locker.release();
                return (THREAD_DEFAULT_RET);
            }
            read_data_len += read_len;
            if ('\n' == buff[read_data_len - 1])
            {
                buff[read_data_len - 1] = '\0';
                break;
            }
        }

        s_stdout_locker.acquire();
        std::cout << "[server]     pipe_r read data [" << buff << "]" << std::endl;
        s_stdout_locker.release();

        if (0 == strcmp("exit", buff))
        {
            break;
        }
    }

    r_pipe.release();

    return (THREAD_DEFAULT_RET);
}

static void test_read_named_pipe(void)
{
    if (goofer_create_detached_thread(test_read_named_pipe_client, nullptr))
    {
        test_read_named_pipe_server(nullptr);
    }
}

static thread_return_t GOOFER_STDCALL test_write_named_pipe_client(thread_argument_t)
{
    NamedPipe w_pipe;

    if (!w_pipe.acquire("name_w", false, true, false))
    {
        s_stdout_locker.acquire();
        std::cout << "[client] create named pipe for write failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }

    char buff[64] = { 0x0 };
    const std::size_t buff_size = sizeof(buff) - 1;

    while (true)
    {
        std::size_t read_data_len = 0;
        while (read_data_len < buff_size)
        {
            std::size_t read_len = 0;
            if (!w_pipe.read(buff + read_data_len, 1, read_len))
            {
                s_stdout_locker.acquire();
                std::cout << "[client] named pipe read failed" << std::endl;
                s_stdout_locker.release();
                return (THREAD_DEFAULT_RET);
            }
            read_data_len += read_len;
            if ('\n' == buff[read_data_len - 1])
            {
                buff[read_data_len - 1] = '\0';
                break;
            }
        }

        s_stdout_locker.acquire();
        std::cout << "[client]     pipe_w read data [" << buff << "]" << std::endl;
        s_stdout_locker.release();

        if (0 == strcmp("exit", buff))
        {
            break;
        }
    }

    w_pipe.release();

    return (THREAD_DEFAULT_RET);
}

static thread_return_t GOOFER_STDCALL test_write_named_pipe_server(thread_argument_t)
{
    NamedPipe w_pipe;

    if (!w_pipe.acquire("name_w", true, false, true))
    {
        s_stdout_locker.acquire();
        std::cout << "[server] create named pipe for write failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }

    char normal_data[] = "write pipe test data";
    for (std::size_t index = 0; index < 5; ++index)
    {
        if (!w_pipe.write(normal_data, sizeof(normal_data) - 1))
        {
            s_stdout_locker.acquire();
            std::cout << "[server] named pipe write failed" << std::endl;
            s_stdout_locker.release();
            return (THREAD_DEFAULT_RET);
        }
        if (!w_pipe.write("\n", 1))
        {
            s_stdout_locker.acquire();
            std::cout << "[server] named pipe write failed" << std::endl;
            s_stdout_locker.release();
            return (THREAD_DEFAULT_RET);
        }
        s_stdout_locker.acquire();
        std::cout << "[server]     pipe_w write data [" << normal_data << "]" << std::endl;
        s_stdout_locker.release();
    }

    char quit_data[] = "exit";
    if (!w_pipe.write(quit_data, sizeof(quit_data) - 1))
    {
        s_stdout_locker.acquire();
        std::cout << "[server] named pipe write failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }
    if (!w_pipe.write("\n", 1))
    {
        s_stdout_locker.acquire();
        std::cout << "[server] named pipe write failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }

    s_stdout_locker.acquire();
    std::cout << "[server]     pipe_w write data [" << quit_data << "]" << std::endl;
    s_stdout_locker.release();

    w_pipe.release();

    return (THREAD_DEFAULT_RET);
}

static void test_write_named_pipe(void)
{
    if (goofer_create_detached_thread(test_write_named_pipe_client, nullptr))
    {
        test_write_named_pipe_server(nullptr);
    }
}

static thread_return_t GOOFER_STDCALL test_read_write_named_pipe_client(thread_argument_t)
{
    NamedPipe rw_pipe;

    if (!rw_pipe.acquire("name_rw", false, true, true))
    {
        s_stdout_locker.acquire();
        std::cout << "[client] create named pipe for read/write failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }

    const std::size_t test_data_len = 16; // "hello named pipe"
    char data[test_data_len + 1] = { 0x0 };

    for (std::size_t index = 0; index < 5; ++index)
    {
        s_stdout_locker.acquire();
        std::cout << "[client] round " << index + 1 << std::endl;
        s_stdout_locker.release();

        std::size_t read_data_len = 0;
        while (read_data_len < test_data_len)
        {
            std::size_t read_len = 0;
            if (!rw_pipe.read(data + read_data_len, test_data_len - read_data_len, read_len))
            {
                s_stdout_locker.acquire();
                std::cout << "[client] named pipe read failed" << std::endl;
                s_stdout_locker.release();
                return (THREAD_DEFAULT_RET);
            }
            read_data_len += read_len;
        }

        s_stdout_locker.acquire();
        std::cout << "[client]     pipe_rw read data [" << data << "]" << std::endl;
        s_stdout_locker.release();

        if (!rw_pipe.write(data, test_data_len))
        {
            s_stdout_locker.acquire();
            std::cout << "[client] named pipe write failed" << std::endl;
            s_stdout_locker.release();
            return (THREAD_DEFAULT_RET);
        }

        s_stdout_locker.acquire();
        std::cout << "[client]     pipe_rw write data [" << data << "]" << std::endl;
        s_stdout_locker.release();
    }

    rw_pipe.release();

    return (THREAD_DEFAULT_RET);
}

static thread_return_t GOOFER_STDCALL test_read_write_named_pipe_server(thread_argument_t)
{
    NamedPipe rw_pipe;

    if (!rw_pipe.acquire("name_rw", true, true, true))
    {
        s_stdout_locker.acquire();
        std::cout << "[server] create named pipe for read/write failed" << std::endl;
        s_stdout_locker.release();
        return (THREAD_DEFAULT_RET);
    }

    char write_data[] = "hello named pipe";
    char read_data[sizeof(write_data)] = { 0x0 };

    for (std::size_t index = 0; index < 5; ++index)
    {
        s_stdout_locker.acquire();
        std::cout << "[server] round " << index + 1 << std::endl;
        s_stdout_locker.release();

        const std::size_t write_data_len = sizeof(write_data) - 1;
        if (!rw_pipe.write(write_data, write_data_len))
        {
            s_stdout_locker.acquire();
            std::cout << "[server] named pipe write failed" << std::endl;
            s_stdout_locker.release();
            return (THREAD_DEFAULT_RET);
        }

        s_stdout_locker.acquire();
        std::cout << "[server]     pipe_rw write data [" << write_data << "]" << std::endl;
        s_stdout_locker.release();

        std::size_t read_data_len = 0;
        while (read_data_len < write_data_len)
        {
            std::size_t read_len = 0;
            if (!rw_pipe.read(read_data + read_data_len, write_data_len - read_data_len, read_len))
            {
                s_stdout_locker.acquire();
                std::cout << "[server] named pipe read failed" << std::endl;
                s_stdout_locker.release();
                return (THREAD_DEFAULT_RET);
            }
            read_data_len += read_len;
        }

        s_stdout_locker.acquire();
        std::cout << "[server]     pipe_rw read data [" << read_data << "]" << std::endl;
        s_stdout_locker.release();

        if (0 != strcmp(write_data, read_data))
        {
            s_stdout_locker.acquire();
            std::cout << "[server] unexpected: pipe_rw read data not equal write data" << std::endl;
            s_stdout_locker.release();
            return (THREAD_DEFAULT_RET);
        }
    }

    rw_pipe.release();

    return (THREAD_DEFAULT_RET);
}

static void test_read_write_named_pipe(void)
{
    if (goofer_create_detached_thread(test_read_write_named_pipe_client, nullptr))
    {
        test_read_write_named_pipe_server(nullptr);
    }
}

static void test_named_pipe(void)
{
    test_read_named_pipe();
    test_write_named_pipe();
    test_read_write_named_pipe();
}

void test_pipe(void)
{
    test_anonymous_pipe();
    test_named_pipe();
}
