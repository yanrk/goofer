/********************************************************
 * Description : conversions of binary and hexadecimal
 * Data        : 2013-05-19 11:53:27
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_HEX_H
#define GOOFER_HEX_H


#include <string>
#include <vector>
#include "common/common.h"

NAMESPACE_GOOFER_BEGIN

#define HEX_ENCODE_SIZE(in_len) ((in_len) * 2 + 1)
#define HEX_DECODE_SIZE(in_len) ((in_len) / 2 + 1)

GOOFER_CXX_API(bool) binary_to_hex(const unsigned char src, char dst[2], bool lower = true);

GOOFER_CXX_API(bool) hex_to_binary(const char src[2], unsigned char * dst);

GOOFER_CXX_API(bool) hex_encode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len, bool lower = true);

GOOFER_CXX_API(bool) hex_decode(const char * src, size_t src_len, unsigned char * dst, size_t dst_len);

GOOFER_CXX_API(bool) hex_encode(const char * src, char * dst, size_t dst_len, bool lower = true);

GOOFER_CXX_API(bool) hex_decode(const char * src, unsigned char * dst, size_t dst_len);

GOOFER_CXX_API(bool) hex_decode(const char * src, char * dst, size_t dst_len);

GOOFER_CXX_API(bool) hex_encode(const void * src, size_t src_len, std::string & dst, bool lower = true);

GOOFER_CXX_API(bool) hex_encode(const char * src, std::string & dst, bool lower = true);

GOOFER_CXX_API(bool) hex_decode(const std::string & src, std::vector<unsigned char> & dst);

GOOFER_CXX_API(bool) hex_encode(const void * src, size_t src_len, std::vector<char> & dst, bool lower = true);

GOOFER_CXX_API(bool) hex_encode(const char * src, std::vector<char> & dst, bool lower = true);

GOOFER_CXX_API(bool) hex_decode(const std::vector<char> & src, std::vector<unsigned char> & dst);

NAMESPACE_GOOFER_END


#endif // GOOFER_HEX_H
