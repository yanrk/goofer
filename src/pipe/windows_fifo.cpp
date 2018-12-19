/********************************************************
 * Description : named pipe class in windows
 * Data        : 2018-12-18 17:48:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifdef _MSC_VER


#include "pipe/windows_fifo.h"

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

WindowsNamedPipe::WindowsNamedPipe()
    : m_is_creator(false)
    , m_is_reader(false)
    , m_is_writer(false)
    , m_pipe_handle(INVALID_HANDLE_VALUE)
{

}

WindowsNamedPipe::~WindowsNamedPipe()
{
    release();
}

bool WindowsNamedPipe::acquire(const std::string & pipe_name, bool is_creator, bool is_reader, bool is_writer)
{
    if (INVALID_HANDLE_VALUE != m_pipe_handle || pipe_name.empty() || (!is_reader && !is_writer))
    {
        return (false);
    }
    m_is_creator = is_creator;
    m_is_reader = is_reader;
    m_is_writer = is_writer;
    const std::string full_pipe_name("\\\\.\\pipe\\" + pipe_name);
    if (is_creator)
    {
        m_pipe_handle = CreateNamedPipeA(full_pipe_name.c_str(), ((is_reader ? PIPE_ACCESS_INBOUND : 0) | (is_writer ? PIPE_ACCESS_OUTBOUND : 0)), (PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_REJECT_REMOTE_CLIENTS), PIPE_UNLIMITED_INSTANCES, 4096, 4096, 0, nullptr);
        if (INVALID_HANDLE_VALUE == m_pipe_handle)
        {
            return (false);
        }
        if (!ConnectNamedPipe(m_pipe_handle, nullptr) && (GetLastError() != ERROR_PIPE_CONNECTED))
        {
            release();
            return (false);
        }
    }
    else
    {
        while (true)
        {
            m_pipe_handle = CreateFileA(full_pipe_name.c_str(), ((is_reader ? GENERIC_READ : 0) | (is_writer ? GENERIC_WRITE : 0)), 0, nullptr, OPEN_EXISTING, 0, nullptr);
            if (INVALID_HANDLE_VALUE != m_pipe_handle)
            {
                break;
            }
            if (ERROR_FILE_NOT_FOUND != GetLastError())
            {
                return (false);
            }
            if (!WaitNamedPipeA(full_pipe_name.c_str(), 1000 * 30) && GetLastError() != ERROR_PIPE_BUSY && GetLastError() != ERROR_SEM_TIMEOUT)
            {
                if (ERROR_FILE_NOT_FOUND == GetLastError())
                {
                    Sleep(50);
                }
                else
                {
                    return (false);
                }
            }
        }
        if (is_writer)
        {
            DWORD pipe_mode = PIPE_WAIT | PIPE_READMODE_BYTE;
            if (!SetNamedPipeHandleState(m_pipe_handle, &pipe_mode, nullptr, nullptr))
            {
                int a = GetLastError();
                release();
                return (false);
            }
        }
    }
    return (true);
}

void WindowsNamedPipe::release()
{
    if (INVALID_HANDLE_VALUE != m_pipe_handle)
    {
        if (m_is_creator)
        {
            DisconnectNamedPipe(m_pipe_handle);
        }
        CloseHandle(m_pipe_handle);
        m_pipe_handle = INVALID_HANDLE_VALUE;
    }
}

bool WindowsNamedPipe::read(char * buff, size_t buff_size, size_t & read_len)
{
    return (m_is_reader && read_pipe(m_pipe_handle, buff, buff_size, read_len));
}

bool WindowsNamedPipe::write(const char * data, size_t data_len)
{
    return (m_is_writer && write_pipe(m_pipe_handle, data, data_len));
}

NAMESPACE_GOOFER_END


#endif // _MSC_VER
