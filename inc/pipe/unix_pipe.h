/********************************************************
 * Description : anonymous pipe class in unix
 * Data        : 2018-11-26 14:09:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_UNIX_ANONYMOUS_PIPE_H
#define GOOFER_UNIX_ANONYMOUS_PIPE_H


#ifndef _MSC_VER


#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API UnixAnonymousPipe : private Uncopy
{
public:
    explicit UnixAnonymousPipe();
    ~UnixAnonymousPipe();

public:
    bool acquire(bool parent_read_child_write, bool parent_write_child_read);
    void release();

public:
    bool read(char * buff, size_t buff_size, size_t & read_len);
    bool write(const char * data, size_t data_len);

public:
    void get_parent_reader(size_t & parent_reader);
    void get_parent_writer(size_t & parent_writer);
    void get_child_reader(size_t & child_reader);
    void get_child_writer(size_t & child_writer);
    void get(size_t & parent_reader, size_t & parent_writer, size_t & child_reader, size_t & child_writer);

public:
    void close_parent_reader();
    void close_parent_writer();
    void close_child_reader();
    void close_child_writer();
    void close_parent_endpoint();
    void close_child_endpoint();

private:
    int                         m_parent_read_child_write_pipe[2];
    int                         m_parent_write_child_read_pipe[2];
};

NAMESPACE_GOOFER_END


#endif // _MSC_VER


#endif // GOOFER_UNIX_ANONYMOUS_PIPE_H
