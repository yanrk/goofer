/********************************************************
 * Description : named pipe class in windows
 * Date        : 2018-12-18 17:48:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_WINDOWS_NAMED_PIPE_H
#define GOOFER_WINDOWS_NAMED_PIPE_H


#include "common/common.h"

#ifdef GOOFER_OS_IS_WIN


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API WindowsNamedPipe : private Uncopy
{
public:
    explicit WindowsNamedPipe();
    ~WindowsNamedPipe();

public:
    bool acquire(const std::string & pipe_name, bool is_creator, bool is_reader, bool is_writer);
    void release();

public:
    bool read(char * buff, size_t buff_size, size_t & read_len);
    bool write(const char * data, size_t data_len);

private:
    bool                        m_is_creator;
    bool                        m_is_reader;
    bool                        m_is_writer;
    HANDLE                      m_pipe_handle;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_OS_IS_WIN


#endif // GOOFER_WINDOWS_NAMED_PIPE_H
