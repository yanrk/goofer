/********************************************************
 * Description : named pipe class in unix
 * Date        : 2018-12-18 19:45:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef _MSC_VER


#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "pipe/unix_fifo.h"

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

UnixNamedPipe::UnixNamedPipe()
    : m_pipe_reader(-1)
    , m_pipe_writer(-1)
{

}

UnixNamedPipe::~UnixNamedPipe()
{
    release();
}

bool UnixNamedPipe::acquire(const std::string & pipe_name, bool is_creator, bool is_reader, bool is_writer)
{
    if (-1 != m_pipe_reader || -1 != m_pipe_writer || pipe_name.empty() || (!is_reader && !is_writer))
    {
        return (false);
    }
    const std::string read_pipe_name("/tmp/" + pipe_name + "_reader");
    const std::string write_pipe_name("/tmp/" + pipe_name + "_writer");
    if (is_creator)
    {
        if (is_reader)
        {
            if ((0 != mkfifo(read_pipe_name.c_str(), S_IFIFO | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) && (EEXIST != errno))
            {
                release();
                return (false);
            }
            m_pipe_reader = open(read_pipe_name.c_str(), O_RDONLY, S_IREAD);
            if (-1 == m_pipe_reader)
            {
                release();
                return (false);
            }
        }
        if (is_writer)
        {
            if ((0 != mkfifo(write_pipe_name.c_str(), S_IFIFO | 0666)) && (EEXIST != errno))
            {
                release();
                return (false);
            }
            m_pipe_writer = open(write_pipe_name.c_str(), O_WRONLY | O_TRUNC, S_IWRITE);
            if (-1 == m_pipe_writer)
            {
                release();
                return (false);
            }
        }
    }
    else
    {
        if (is_writer)
        {
            m_pipe_writer = open(read_pipe_name.c_str(), O_WRONLY | O_TRUNC, S_IWRITE);
            if (-1 == m_pipe_writer)
            {
                release();
                return (false);
            }
        }
        if (is_reader)
        {
            m_pipe_reader = open(write_pipe_name.c_str(), O_RDONLY, S_IREAD);
            if (-1 == m_pipe_reader)
            {
                release();
                return (false);
            }
        }
    }
    return (true);
}

void UnixNamedPipe::release()
{
    if (-1 != m_pipe_reader)
    {
        close(m_pipe_reader);
        m_pipe_reader = -1;
    }
    if (-1 != m_pipe_writer)
    {
        close(m_pipe_writer);
        m_pipe_writer = -1;
    }
}

bool UnixNamedPipe::read(char * buff, size_t buff_size, size_t & read_len)
{
    return (read_pipe(m_pipe_reader, buff, buff_size, read_len));
}

bool UnixNamedPipe::write(const char * data, size_t data_len)
{
    return (write_pipe(m_pipe_writer, data, data_len));
}

NAMESPACE_GOOFER_END


#endif // _MSC_VER
