/********************************************************
 * Description : byte stream operator classes
 * Date        : 2013-05-19 14:04:57
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_BYTE_STREAM_H
#define GOOFER_BYTE_STREAM_H


#include <string>
#include <cstdint>
#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API ByteStream : private Uncopy
{
public:
    ByteStream(size_t len);
    ~ByteStream();

public:
    static void host_to_net(void * obj, size_t size);
    static void net_to_host(void * obj, size_t size);

    bool seek(size_t offset);
    size_t tell();
    size_t left();

protected:
    void reset(size_t len);

protected:
    size_t m_length;
    size_t m_offset;
};


class GOOFER_API OBStream : public ByteStream
{
public:
    OBStream(unsigned char * byte, size_t len);
    ~OBStream();

public:
    void reset(unsigned char * byte, size_t len);

public:
    /* this function is different, it will not change inner-byte-offset */
    void write_bytes(size_t offset, const void * obj, size_t size);

public:
    OBStream & write(const void * obj, size_t size);

public:
    OBStream & operator << (bool);
    OBStream & operator << (int8_t);
    OBStream & operator << (uint8_t);
    OBStream & operator << (int16_t);
    OBStream & operator << (uint16_t);
    OBStream & operator << (int32_t);
    OBStream & operator << (uint32_t);
    OBStream & operator << (int64_t);
    OBStream & operator << (uint64_t);
    OBStream & operator << (float);
    OBStream & operator << (double);
    OBStream & operator << (const std::string &);

private:
    void check_write(size_t);
    void do_write_integer(uint64_t, size_t, unsigned char);
    void write_integer(uint64_t);

private:
    unsigned char * m_byte;
};


class GOOFER_API IBStream : public ByteStream
{
public:
    IBStream(const unsigned char * byte, size_t len);
    ~IBStream();

public:
    void reset(const unsigned char * byte, size_t len);

public:
    /* this function is different, it will not change inner-byte-offset */
    void read_bytes(size_t offset, void * obj, size_t size);

public:
    IBStream & read(void * obj, size_t size);

public:
    IBStream & operator >> (bool &);
    IBStream & operator >> (int8_t &);
    IBStream & operator >> (uint8_t &);
    IBStream & operator >> (int16_t &);
    IBStream & operator >> (uint16_t &);
    IBStream & operator >> (int32_t &);
    IBStream & operator >> (uint32_t &);
    IBStream & operator >> (int64_t &);
    IBStream & operator >> (uint64_t &);
    IBStream & operator >> (float &);
    IBStream & operator >> (double &);
    IBStream & operator >> (std::string &);

private:
    void check_read(size_t);
    uint64_t do_read_integer(unsigned char, size_t);
    uint64_t read_integer();

private:
    const unsigned char * m_byte;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_BYTE_STREAM_H
