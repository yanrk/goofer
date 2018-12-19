/********************************************************
 * Description : named pipe class in unix
 * Data        : 2018-12-18 19:45:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_UNIX_NAMED_PIPE_H
#define GOOFER_UNIX_NAMED_PIPE_H


#ifndef _MSC_VER


#include <string>

#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API UnixNamedPipe : private Uncopy
{
public:
    explicit UnixNamedPipe();
    ~UnixNamedPipe();

public:
    bool acquire(const std::string & pipe_name, bool is_creator, bool is_reader, bool is_writer);
    void release();

public:
    bool read(char * buff, size_t buff_size, size_t & read_len);
    bool write(const char * data, size_t data_len);

private:
    int                         m_pipe_reader;
    int                         m_pipe_writer;
};

NAMESPACE_GOOFER_END


#endif // _MSC_VER


#endif // GOOFER_UNIX_NAMED_PIPE_H
