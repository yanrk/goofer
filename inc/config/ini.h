/********************************************************
 * Description : ini parse class
 * Date        : 2013-07-27 16:11:54
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_INI_H
#define GOOFER_INI_H


#include <string>
#include <list>
#include <map>
#include "common/common.h"
#include "utility/uncopy.h"
#include "string/string.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API Ini : private Uncopy
{
public:
    Ini();
    ~Ini();

public:
    bool load(const std::string & file_name, char comment_char = ';', bool support_modify = false, bool loose = false);
    bool save(const std::string & file_name);
    bool save();
    void clear();

public:
    bool set_document(const std::string & document, char comment_char = ';', bool support_modify = false, bool loose = false);
    std::string get_document() const;

public:
    bool get_value(const std::string & app_name, const std::string & key_name, std::string & key_value) const;
    bool get_value(const std::string & app_name, const std::string & key_name, char * key_value, size_t key_value_size) const;
    bool set_value(const std::string & app_name, const std::string & key_name, const std::string & key_value);
    bool set_value(const std::string & app_name, const std::string & key_name, const char * key_value);
    template <typename T> bool get_value(const std::string & app_name, const std::string & key_name, T & key_value) const;
    template <typename T> bool set_value(const std::string & app_name, const std::string & key_name, T key_value);

private:
    void parse(std::istream & is);
    void build(std::ostream & os) const;

private:
    void add_app_node(const std::string & app_name);
    void add_key_node(const std::string & app_name, const std::string & key_name, const std::string & key_value);
    void save_app_name(std::ostream & os, const std::string & app_name) const;
    void save_key_value(std::ostream & os, const std::string & key_name, const std::string & key_value) const;

private:
    struct key_node_t
    {
        std::string              m_key_name;
        std::string              m_key_value;

        key_node_t(const std::string & key_name, const std::string & key_value);
        bool operator == (const std::string & key_name) const;
    };

    struct app_node_t
    {
        std::string              m_app_name;
        std::list<key_node_t>    m_key_list;

        app_node_t(const std::string & app_name);
        bool operator == (const std::string & app_name) const;
    };

private:
    static const char *                                         s_app_head_format[2];
    static const char *                                         s_app_tail_format[2];
    static const char *                                         s_key_equal_format[2];

private:
    std::string                                                 m_file_name;
    char                                                        m_comment_char;
    size_t                                                      m_format_mode;
    std::map<std::pair<std::string, std::string>, std::string>  m_pair_map;
    std::list<app_node_t>                                       m_app_list;
};

template <typename T>
bool Ini::get_value(const std::string & app_name, const std::string & key_name, T & key_value) const
{
    std::string str_value;
    if (!get_value(app_name, key_name, str_value))
    {
        return (false);
    }
    return (goofer_string_to_type(str_value, key_value));
}

template <typename T>
bool Ini::set_value(const std::string & app_name, const std::string & key_name, T key_value)
{
    std::string str_value;
    if (!goofer_type_to_string(key_value, str_value))
    {
        return (false);
    }
    return (set_value(app_name, key_name, str_value));
}

NAMESPACE_GOOFER_END


#endif // GOOFER_INI_H
