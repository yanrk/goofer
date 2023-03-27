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

#include <cctype>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "config/ini.h"
#include "charset/charset.h"
#include "filesystem/sys_io.h"

NAMESPACE_GOOFER_BEGIN

const char * Ini::s_app_head_format[] = { "[", "[ " };
const char * Ini::s_app_tail_format[] = { "]", " ]" };
const char * Ini::s_key_equal_format[] = { "=", " = " };

Ini::key_node_t::key_node_t(const std::string & key_name, const std::string & key_value)
    : m_key_name(key_name)
    , m_key_value(key_value)
{

}

bool Ini::key_node_t::operator == (const std::string & key_name) const
{
    return (key_name == m_key_name);
}

Ini::app_node_t::app_node_t(const std::string & app_name)
    : m_app_name(app_name)
    , m_key_list()
{

}

bool Ini::app_node_t::operator == (const std::string & app_name) const
{
    return (app_name == m_app_name);
}

Ini::Ini()
    : m_file_name()
    , m_comment_char(';')
    , m_format_mode(0)
    , m_pair_map()
    , m_app_list()
{

}

Ini::~Ini()
{

}

void Ini::parse(std::istream & is)
{
    std::string app_name;
    std::string message;
    while (!is.eof())
    {
        message.clear();
        std::getline(is, message);
        goofer_string_trim(message);
        std::string::size_type comment_pos = message.find(m_comment_char);
        if (std::string::npos != comment_pos)
        {
            message.resize(comment_pos);
        }
        if (message.empty())
        {
            continue;
        }

        if ('[' == message[0])
        {
            std::string::iterator iter_b = message.begin() + 1;
            std::string::iterator iter_e = message.end() - 1;
            if (']' != *iter_e)
            {
                iter_e = message.end();
            }
            std::string(iter_b, iter_e).swap(app_name);
            goofer_string_trim(app_name);

            add_app_node(app_name);
        }
        else
        {
            std::string::iterator iter_k_e;
            std::string::iterator iter_v_s;
            std::string::size_type equal_pos = message.find('=');
            if (std::string::npos == equal_pos)
            {
                iter_k_e = message.end();
                iter_v_s = message.end();
            }
            else
            {
                iter_k_e = message.begin() + equal_pos;
                iter_v_s = message.begin() + equal_pos + 1;
            }

            std::string key_name(message.begin(), iter_k_e);
            std::string key_value(iter_v_s, message.end());
            goofer_string_trim(key_name);
            goofer_string_trim(key_value);

            if (key_name.empty())
            {
                continue;
            }

            if (!m_pair_map.insert(std::make_pair(std::make_pair(app_name, key_name), key_value)).second)
            {
                continue;
            }

            add_app_node(app_name);
            add_key_node(app_name, key_name, key_value);
        }
    }
}

void Ini::build(std::ostream & os) const
{
    std::list<app_node_t>::const_iterator app_iter = m_app_list.begin();
    while (m_app_list.end() != app_iter)
    {
        save_app_name(os, app_iter->m_app_name);
        std::list<key_node_t>::const_iterator key_iter = app_iter->m_key_list.begin();
        while (app_iter->m_key_list.end() != key_iter)
        {
            save_key_value(os, key_iter->m_key_name, key_iter->m_key_value);
            ++key_iter;
        }
        ++app_iter;
    }
}

bool Ini::load(const std::string & file_name, char comment_char, bool support_modify, bool loose)
{
    clear();

    if (isspace(comment_char))
    {
        comment_char = ';';
    }

    m_file_name = utf8_to_ansi(file_name);
    m_comment_char = comment_char;
    m_format_mode = (loose ? 1 : 0);

    std::ifstream ifs(m_file_name.c_str());
    if (!ifs.is_open())
    {
        return (support_modify);
    }

    parse(ifs);

    ifs.close();

    return (true);
}

bool Ini::set_document(const std::string & document, char comment_char, bool support_modify, bool loose)
{
    const std::string file_name = m_file_name;

    clear();

    if (isspace(comment_char))
    {
        comment_char = ';';
    }

    m_file_name = file_name;
    m_comment_char = comment_char;
    m_format_mode = (loose ? 1 : 0);

    if (document.empty())
    {
        return (true);
    }

    std::istringstream iss(document);

    parse(iss);

    return (true);
}

bool Ini::save(const std::string & file_name)
{
    const std::string tmp_file(file_name + "_tmp.ini");

    goofer_rename(file_name.c_str(), tmp_file.c_str());

    std::ofstream ofs(file_name.c_str(), std::ios::trunc);
    if (!ofs.is_open())
    {
        return (false);
    }

    build(ofs);

    ofs.close();

    goofer_unlink(tmp_file.c_str());

    return (true);
}

bool Ini::save()
{
    if (m_file_name.empty())
    {
        return (false);
    }

    if (!save(m_file_name))
    {
        return (false);
    }

    return (true);
}

std::string Ini::get_document() const
{
    std::ostringstream oss;

    build(oss);

    return (oss.str());
}

bool Ini::get_value(const std::string & app_name, const std::string & key_name, std::string & key_value) const
{
    if (key_name.empty())
    {
        return (false);
    }

    std::map<std::pair<std::string, std::string>, std::string>::const_iterator pair_iter = m_pair_map.find(std::make_pair(app_name, key_name));
    if (m_pair_map.end() != pair_iter)
    {
        key_value = pair_iter->second;
        return (true);
    }
    else
    {
        return (false);
    }
}

bool Ini::get_value(const std::string & app_name, const std::string & key_name, char * key_value, size_t key_value_size) const
{
    std::string str_key_value;
    if (!get_value(app_name, key_name, str_key_value))
    {
        return (false);
    }
    if (nullptr == key_value || str_key_value.size() >= key_value_size)
    {
        return (false);
    }
    strncpy(key_value, str_key_value.c_str(), key_value_size);
    return (true);
}

bool Ini::set_value(const std::string & app_name, const std::string & key_name, const std::string & key_value)
{
    if (key_name.empty())
    {
        return (false);
    }

    std::list<app_node_t>::iterator app_iter = std::find(m_app_list.begin(), m_app_list.end(), app_name);
    if (m_app_list.end() == app_iter)
    {
        m_app_list.push_back(app_node_t(app_name));
        app_iter = m_app_list.end();
        --app_iter;
    }
    std::list<key_node_t> & key_list = app_iter->m_key_list;
    std::list<key_node_t>::iterator key_iter = std::find(key_list.begin(), key_list.end(), key_name);
    if (key_list.end() == key_iter)
    {
        key_list.push_back(key_node_t(key_name, key_value));
    }
    else
    {
        key_iter->m_key_value = key_value;
    }

    m_pair_map[std::make_pair(app_name, key_name)] = key_value;

    return (true);
}

bool Ini::set_value(const std::string & app_name, const std::string & key_name, const char * key_value)
{
    if (nullptr == key_value)
    {
        return (false);
    }
    const std::string str_key_value(key_value);
    if (!set_value(app_name, key_name, str_key_value))
    {
        return (false);
    }
    return (true);
}

void Ini::add_app_node(const std::string & app_name)
{
    if (m_app_list.end() != std::find(m_app_list.begin(), m_app_list.end(), app_name))
    {
        return;
    }
    m_app_list.push_back(app_node_t(app_name));
}

void Ini::add_key_node(const std::string & app_name, const std::string & key_name, const std::string & key_value)
{
    std::list<app_node_t>::reverse_iterator app_re_iter = std::find(m_app_list.rbegin(), m_app_list.rend(), app_name);
    if (m_app_list.rend() == app_re_iter)
    {
        return;
    }

    std::list<key_node_t> & key_list = app_re_iter->m_key_list;
    if (key_list.end() != std::find(key_list.begin(), key_list.end(), key_name))
    {
        return;
    }

    key_list.push_back(key_node_t(key_name, key_value));
}

void Ini::clear()
{
    m_file_name.clear();
    m_comment_char = ';';
    m_format_mode = 0;
    m_pair_map.clear();
    m_app_list.clear();
}

void Ini::save_app_name(std::ostream & os, const std::string & app_name) const
{
    if (!app_name.empty())
    {
        os << s_app_head_format[m_format_mode] << app_name << s_app_tail_format[m_format_mode] << std::endl;
    }
}

void Ini::save_key_value(std::ostream & os, const std::string & key_name, const std::string & key_value) const
{
    os << key_name << s_key_equal_format[m_format_mode] << key_value << std::endl;
}

NAMESPACE_GOOFER_END
