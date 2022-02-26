/********************************************************
 * Description : encode and decode of bcd
 * Data        : 2014-03-22 17:03:12
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cstring>
#include <algorithm>
#include "string/bcd.h"

struct legit_bcd_encode_element
{
    bool operator() (char ele) const
    {
        return (ele >= '0' && ele <= '9');
    }
};

struct legit_bcd_decode_element
{
    bool operator() (unsigned char ele) const
    {
        unsigned char hig = (ele & 0xF0);
        unsigned char low = (ele & 0x0F);
        return (hig <= 0x90 && low <= 0x09);
    }
};

NAMESPACE_GOOFER_BEGIN

bool bcd_encode(const char * src, size_t src_len, unsigned char * dst, size_t dst_len, size_t & out_len)
{
    out_len = 0;

    if (nullptr == src || nullptr == dst || BCD_ENCODE_SIZE(src_len) > dst_len)
    {
        return (false);
    }

    if (0 == src_len)
    {
        return (true);
    }

    if (src + src_len != std::find_if_not(src, src + src_len, legit_bcd_encode_element()))
    {
        return (false);
    }

    if (1 == src_len % 2)
    {
        dst[0] = static_cast<unsigned char>(src[0] - '0');
        src += 1;
        src_len -= 1;
        dst += 1;
        out_len += 1;
    }

    while (0 != src_len)
    {
        dst[0] = static_cast<unsigned char>(((src[0] - '0') << 4) | ((src[1] - '0')));
        src += 2;
        src_len -= 2;
        dst += 1;
        out_len += 1;
    }

    return (true);
}

bool bcd_encode(const char * src, unsigned char * dst, size_t dst_len, size_t & out_len)
{
    if (nullptr == src)
    {
        out_len = 0;
        return (false);
    }
    return (bcd_encode(src, strlen(src), dst, dst_len, out_len));
}

bool bcd_decode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len, size_t & out_len)
{
    out_len = 0;

    if (nullptr == src || nullptr == dst)
    {
        return (false);
    }

    if ((BCD_DECODE_SIZE(src_len) - 1 > dst_len) || (src[0] >= 0x10 && BCD_DECODE_SIZE(src_len) > dst_len))
    {
        return (false);
    }

    if (0 == src_len)
    {
        dst[0] = '\0';
        return (true);
    }

    if (src + src_len != std::find_if_not(src, src + src_len, legit_bcd_decode_element()))
    {
        return (false);
    }

    if (src[0] < 0x10)
    {
        dst[0] = static_cast<char>(src[0] + '0');
        src += 1;
        src_len -= 1;
        dst += 1;
        out_len += 1;
    }

    while (0 != src_len)
    {
        dst[0] = static_cast<char>(((src[0] >> 4) & 0x0F) + '0');
        dst[1] = static_cast<char>(((src[0] & 0x0F)) + '0');
        src += 1;
        src_len -= 1;
        dst += 2;
        out_len += 2;
    }

    dst[0] = '\0';

    return (true);
}

bool bcd_decode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len)
{
    size_t out_len = 0;
    return (bcd_decode(src, src_len, dst, dst_len, out_len));
}

bool bcd_encode(const char * src, size_t src_len, std::vector<unsigned char> & dst)
{
    dst.resize(BCD_ENCODE_SIZE(src_len));
    size_t out_len = 0;
    if (bcd_encode(src, src_len, &dst[0], dst.size(), out_len))
    {
        dst.resize(out_len);
        return (true);
    }
    else
    {
        dst.clear();
        return (false);
    }
}

bool bcd_encode(const char * src, std::vector<unsigned char> & dst)
{
    if (nullptr == src)
    {
        dst.clear();
        return (false);
    }
    return (bcd_encode(src, strlen(src), dst));
}

bool bcd_decode(const std::vector<unsigned char> & src, std::string & dst)
{
    size_t src_len = src.size();
    dst.resize(BCD_DECODE_SIZE(src_len));
    size_t out_len = 0;
    if (bcd_decode(src.data(), src_len, &dst[0], dst.size(), out_len))
    {
        dst.resize(out_len);
        return (true);
    }
    else
    {
        dst.clear();
        return (false);
    }
}

bool bcd_decode(const std::vector<unsigned char> & src, std::vector<char> & dst)
{
    size_t src_len = src.size();
    dst.resize(BCD_DECODE_SIZE(src_len));
    size_t out_len = 0;
    if (bcd_decode(src.data(), src_len, &dst[0], dst.size(), out_len))
    {
        dst.resize(out_len);
        return (true);
    }
    else
    {
        dst.clear();
        return (false);
    }
}

NAMESPACE_GOOFER_END
