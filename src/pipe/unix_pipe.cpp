/********************************************************
 * Description : anonymous pipe class in unix
 * Date        : 2018-11-26 14:09:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef _MSC_VER


#include <unistd.h>

#include "pipe/unix_pipe.h"

static bool create_pipe_endpoints(int file_descriptor[2])
{
    file_descriptor[0] = -1;
    file_descriptor[1] = -1;
    return (-1 != pipe(file_descriptor));
}

static void close_pipe_endpoint(int file_descriptor)
{
    if (-1 != file_descriptor)
    {
        close(file_descriptor);
        file_descriptor = -1;
    }
}

static bool read_pipe(int file_descriptor, char * buff, size_t buff_size, size_t & read_len)
{
    read_len = 0;
    if (-1 == file_descriptor || nullptr == buff || 0 == buff_size)
    {
        return (false);
    }
    ssize_t read_size = read(file_descriptor, buff, buff_size);
    if (-1 != read_size)
    {
        read_len = static_cast<size_t>(read_size);
        return (true);
    }
    return (false);
}

static bool write_pipe(int file_descriptor, const char * data, size_t data_len)
{
    if (-1 == file_descriptor || nullptr == data || 0 == data_len)
    {
        return (false);
    }
    while (data_len > 0)
    {
        ssize_t write_size = write(file_descriptor, data, data_len);
        if (-1 != write_size)
        {
            data += static_cast<size_t>(write_size);
            data_len -= static_cast<size_t>(write_size);
        }
        else
        {
            return (false);
        }
    }
    return (true);
}

NAMESPACE_GOOFER_BEGIN

UnixAnonymousPipe::UnixAnonymousPipe()
    : m_parent_read_child_write_pipe()
    , m_parent_write_child_read_pipe()
{
    m_parent_read_child_write_pipe[0] = -1;
    m_parent_read_child_write_pipe[1] = -1;
    m_parent_write_child_read_pipe[0] = -1;
    m_parent_write_child_read_pipe[1] = -1;
}

UnixAnonymousPipe::~UnixAnonymousPipe()
{
    release();
}

bool UnixAnonymousPipe::acquire(bool parent_read_child_write, bool parent_write_child_read)
{
    if (-1 != m_parent_read_child_write_pipe[0] || -1 != m_parent_read_child_write_pipe[1] || -1 != m_parent_write_child_read_pipe[0] || -1 != m_parent_write_child_read_pipe[1])
    {
        return (false);
    }
    if (parent_read_child_write)
    {
        if (!create_pipe_endpoints(m_parent_read_child_write_pipe))
        {
            release();
            return (false);
        }
    }
    if (parent_write_child_read)
    {
        if (!create_pipe_endpoints(m_parent_write_child_read_pipe))
        {
            release();
            return (false);
        }
    }
    return (parent_read_child_write || parent_write_child_read);
}

void UnixAnonymousPipe::release()
{
    close_parent_endpoint();
    close_child_endpoint();
}

bool UnixAnonymousPipe::read(char * buff, size_t buff_size, size_t & read_len)
{
    return (read_pipe(m_parent_read_child_write_pipe[0], buff, buff_size, read_len));
}

bool UnixAnonymousPipe::write(const char * data, size_t data_len)
{
    return (write_pipe(m_parent_write_child_read_pipe[1], data, data_len));
}

void UnixAnonymousPipe::get_parent_reader(size_t & parent_reader)
{
    parent_reader = static_cast<size_t>(m_parent_read_child_write_pipe[0]);
}

void UnixAnonymousPipe::get_parent_writer(size_t & parent_writer)
{
    parent_writer = static_cast<size_t>(m_parent_write_child_read_pipe[1]);
}

void UnixAnonymousPipe::get_child_reader(size_t & child_reader)
{
    child_reader = static_cast<size_t>(m_parent_write_child_read_pipe[0]);
}

void UnixAnonymousPipe::get_child_writer(size_t & child_writer)
{
    child_writer = static_cast<size_t>(m_parent_read_child_write_pipe[1]);
}

void UnixAnonymousPipe::get(size_t & parent_reader, size_t & parent_writer, size_t & child_reader, size_t & child_writer)
{
    get_parent_reader(parent_reader);
    get_parent_writer(parent_writer);
    get_child_reader(child_reader);
    get_child_writer(child_writer);
}

void UnixAnonymousPipe::close_parent_reader()
{
    close_pipe_endpoint(m_parent_read_child_write_pipe[0]);
}

void UnixAnonymousPipe::close_parent_writer()
{
    close_pipe_endpoint(m_parent_write_child_read_pipe[1]);
}

void UnixAnonymousPipe::close_child_reader()
{
    close_pipe_endpoint(m_parent_write_child_read_pipe[0]);
}

void UnixAnonymousPipe::close_child_writer()
{
    close_pipe_endpoint(m_parent_read_child_write_pipe[1]);
}

void UnixAnonymousPipe::close_parent_endpoint()
{
    close_parent_reader();
    close_parent_writer();
}

void UnixAnonymousPipe::close_child_endpoint()
{
    close_child_reader();
    close_child_writer();
}

NAMESPACE_GOOFER_END


#endif // _MSC_VER
