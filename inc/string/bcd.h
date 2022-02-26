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

#ifndef GOOFER_BCD_H
#define GOOFER_BCD_H


#include <string>
#include <vector>
#include "common/common.h"

NAMESPACE_GOOFER_BEGIN

#define BCD_ENCODE_SIZE(in_len) (((in_len) / 2) + 1)
#define BCD_DECODE_SIZE(in_len) (((in_len) * 2) + 1)

GOOFER_CXX_API(bool) bcd_encode(const char * src, size_t src_len, unsigned char * dst, size_t dst_len, size_t & out_len);

GOOFER_CXX_API(bool) bcd_encode(const char * src, unsigned char * dst, size_t dst_len, size_t & out_len);

GOOFER_CXX_API(bool) bcd_decode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len, size_t & out_len);

GOOFER_CXX_API(bool) bcd_decode(const unsigned char * src, size_t src_len, char * dst, size_t dst_len);

GOOFER_CXX_API(bool) bcd_encode(const char * src, size_t src_len, std::vector<unsigned char> & dst);

GOOFER_CXX_API(bool) bcd_encode(const char * src, std::vector<unsigned char> & dst);

GOOFER_CXX_API(bool) bcd_decode(const std::vector<unsigned char> & src, std::string & dst);

GOOFER_CXX_API(bool) bcd_decode(const std::vector<unsigned char> & src, std::vector<char> & dst);

NAMESPACE_GOOFER_END


#endif // GOOFER_BCD_H
