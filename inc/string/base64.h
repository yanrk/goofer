/********************************************************
 * Description : encode and decode of base64
 * Data        : 2013-05-19 10:20:25
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_BASE64_H
#define GOOFER_BASE64_H


#include <string>
#include <vector>
#include "common/common.h"

NAMESPACE_GOOFER_BEGIN

#define BASE64_ENCODE_SIZE(in_len) (((in_len) + 2) / 3 * 4 + 1)
#define BASE64_DECODE_SIZE(in_len) (((in_len) + 3) / 4 * 3 + 1)

GOOFER_CXX_API(bool) base64_encode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len, size_t & out_len);

GOOFER_CXX_API(bool) base64_decode(const char * src, size_t src_len, unsigned char * dst, size_t dst_len, size_t & out_len);

GOOFER_CXX_API(bool) base64_encode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len);

GOOFER_CXX_API(bool) base64_decode(const char * src, unsigned char * dst, size_t dst_len);

GOOFER_CXX_API(bool) base64_encode(const void * src, size_t src_len, char * dst, size_t dst_len);

GOOFER_CXX_API(bool) base64_decode(const char * src, void * dst, size_t dst_len);

GOOFER_CXX_API(bool) base64_encode(const char * src, char * dst, size_t dst_len);

GOOFER_CXX_API(bool) base64_decode(const char * src, char * dst, size_t dst_len);

GOOFER_CXX_API(bool) base64_encode(const void * src, size_t src_len, std::string & dst);

GOOFER_CXX_API(bool) base64_encode(const char * src, std::string & dst);

GOOFER_CXX_API(bool) base64_decode(const std::string & src, std::vector<unsigned char> & dst);

GOOFER_CXX_API(bool) base64_encode(const void * src, size_t src_len, std::vector<char> & dst);

GOOFER_CXX_API(bool) base64_encode(const char * src, std::vector<char> & dst);

GOOFER_CXX_API(bool) base64_decode(const std::vector<char> & src, std::vector<unsigned char> & dst);

NAMESPACE_GOOFER_END


#endif // GOOFER_BASE64_H
