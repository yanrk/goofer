/********************************************************
 * Description : anonymous pipe class in windows
 * Date        : 2018-11-26 11:36:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER


#include "pipe/windows_pipe.h"

static bool create_pipe_endpoints(HANDLE & read_handle, HANDLE & write_handle)
{
    read_handle = INVALID_HANDLE_VALUE;
    write_handle = INVALID_HANDLE_VALUE;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
    return (TRUE == CreatePipe(&read_handle, &write_handle, &sa, 0));
}

static void close_pipe_endpoint(HANDLE & handle)
{
    if (INVALID_HANDLE_VALUE != handle)
    {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
    }
}

static bool read_pipe(HANDLE read_handle, char * buff, size_t buff_size, size_t & read_len)
{
    read_len = 0;
    if (INVALID_HANDLE_VALUE == read_handle || nullptr == buff || 0 == buff_size)
    {
        return (false);
    }
    DWORD read_size = 0;
    if (ReadFile(read_handle, buff, static_cast<DWORD>(buff_size), &read_size, nullptr))
    {
        read_len = static_cast<size_t>(read_size);
        return (true);
    }
    return (ERROR_BROKEN_PIPE == GetLastError());
}

static bool write_pipe(HANDLE write_handle, const char * data, size_t data_len)
{
    if (INVALID_HANDLE_VALUE == write_handle || nullptr == data || 0 == data_len)
    {
        return (false);
    }
    while (data_len > 0)
    {
        DWORD write_size = 0;
        if (WriteFile(write_handle, data, static_cast<DWORD>(data_len), &write_size, nullptr))
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

WindowsAnonymousPipe::WindowsAnonymousPipe()
    : m_parent_read_handle(INVALID_HANDLE_VALUE)
    , m_parent_write_handle(INVALID_HANDLE_VALUE)
    , m_child_read_handle(INVALID_HANDLE_VALUE)
    , m_child_write_handle(INVALID_HANDLE_VALUE)
{

}

WindowsAnonymousPipe::~WindowsAnonymousPipe()
{
    release();
}

bool WindowsAnonymousPipe::acquire(bool parent_read_child_write, bool parent_write_child_read)
{
    if (INVALID_HANDLE_VALUE != m_parent_read_handle || INVALID_HANDLE_VALUE != m_parent_write_handle || INVALID_HANDLE_VALUE != m_child_read_handle || INVALID_HANDLE_VALUE != m_child_write_handle)
    {
        return (false);
    }
    if (parent_read_child_write)
    {
        if (!create_pipe_endpoints(m_parent_read_handle, m_child_write_handle))
        {
            release();
            return (false);
        }
    }
    if (parent_write_child_read)
    {
        if (!create_pipe_endpoints(m_child_read_handle, m_parent_write_handle))
        {
            release();
            return (false);
        }
    }
    return (parent_read_child_write || parent_write_child_read);
}

void WindowsAnonymousPipe::release()
{
    close_parent_endpoint();
    close_child_endpoint();
}

bool WindowsAnonymousPipe::read(char * buff, size_t buff_size, size_t & read_len)
{
    return (read_pipe(m_parent_read_handle, buff, buff_size, read_len));
}

bool WindowsAnonymousPipe::write(const char * data, size_t data_len)
{
    return (write_pipe(m_parent_write_handle, data, data_len));
}

void WindowsAnonymousPipe::get_parent_reader(size_t & parent_reader)
{
    parent_reader = reinterpret_cast<size_t>(m_parent_read_handle);
}

void WindowsAnonymousPipe::get_parent_writer(size_t & parent_writer)
{
    parent_writer = reinterpret_cast<size_t>(m_parent_write_handle);
}

void WindowsAnonymousPipe::get_child_reader(size_t & child_reader)
{
    child_reader = reinterpret_cast<size_t>(m_child_read_handle);
}

void WindowsAnonymousPipe::get_child_writer(size_t & child_writer)
{
    child_writer = reinterpret_cast<size_t>(m_child_write_handle);
}

void WindowsAnonymousPipe::get(size_t & parent_reader, size_t & parent_writer, size_t & child_reader, size_t & child_writer)
{
    get_parent_reader(parent_reader);
    get_parent_writer(parent_writer);
    get_child_reader(child_reader);
    get_child_writer(child_writer);
}

void WindowsAnonymousPipe::close_parent_reader()
{
    close_pipe_endpoint(m_parent_read_handle);
}

void WindowsAnonymousPipe::close_parent_writer()
{
    close_pipe_endpoint(m_parent_write_handle);
}

void WindowsAnonymousPipe::close_child_reader()
{
    close_pipe_endpoint(m_child_read_handle);
}

void WindowsAnonymousPipe::close_child_writer()
{
    close_pipe_endpoint(m_child_write_handle);
}

void WindowsAnonymousPipe::close_parent_endpoint()
{
    close_parent_reader();
    close_parent_writer();
}

void WindowsAnonymousPipe::close_child_endpoint()
{
    close_child_reader();
    close_child_writer();
}

NAMESPACE_GOOFER_END


#endif // _MSC_VER
