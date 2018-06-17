/********************************************************
 * Description : splitter of string
 * Data        : 2014-01-03 18:50:41
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_SPLITTER_H
#define GOOFER_SPLITTER_H


#include <string>
#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API StringSplitter : private Uncopy
{
public:
    StringSplitter(
        const std::string & string_set, 
        const std::string & filter_set = g_blank_character_set, 
        const std::string & splitter_set = ","
    );

public:
    size_t count_bound() const;

public:
    bool has_element() const;
    std::string pop_element();

public:
    bool eof() const;
    StringSplitter & operator >> (std::string & element);

public:
    void rewind();

private:
    std::string                   m_string_set;
    std::string                   m_filter_set;
    std::string                   m_splitter_set;
    std::string::const_iterator   m_iter_b;
    std::string::const_iterator   m_iter_e;
    bool                          m_is_eof;
    size_t                        m_count_bound;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_SPLITTER_H
