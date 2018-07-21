/********************************************************
 * Description : json parse class which base on jsoncpp
 * Data        : 2018-06-17 20:54:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <cassert>
#include <cstring>
#include <fstream>
#include "json/json.h"
#include "string/string.h"
#include "charset/charset.h"
#include "config/json.h"

class JsonImpl
{
public:
    JsonImpl();
    ~JsonImpl();

private:
    JsonImpl(const JsonImpl & other);
    JsonImpl & operator = (const JsonImpl & other);

public:
    bool into_element(size_t element_index);
    bool into_element(const char * element_name);
    bool outof_element();

public:
    Goofer::json_value_t::v_t get_type();
    size_t get_size();

public: /* read */
    bool load(const char * file_name);
    bool set_document(const char * document);
    bool find_element(const char * element_name);
    bool get_element(const char * element_name, std::string & element_value);
    bool get_element(const char * element_name, char * element_value, size_t element_value_size);
    bool get_element(const char * element_name, std::list<std::string> & element_value_list);
    bool get_sub_document(const char * element_name, std::string & sub_document, bool format = true);
    bool get_sub_document(size_t element_index, std::string & sub_document, bool format = true);

public: /* write */
    bool save(const char * file_name, bool format = true);
    bool get_document(std::string & document, bool format = true);
    bool save_sub_document(const char * element_name, const char * file_name, bool format = true);
    bool save_sub_document(size_t element_index, const char * file_name, bool format = true);
    bool set_sub_document(const char * element_name, const char * sub_document);
    bool set_sub_document(size_t element_index, const char * sub_document);
    bool add_array(const char * element_name);
    bool add_element(size_t element_index);
    bool add_element(const char * element_name);
    bool add_element(const char * element_name, const char * element_value);
    bool add_element(const char * element_name, const std::string & element_value);
    bool add_element(const char * element_name, const std::list<std::string> & element_value_list);
    bool set_element(const char * element_name, const char * element_value);
    bool set_element(const char * element_name, const std::string & element_value);
    bool set_element(const char * element_name, const std::list<std::string> & element_value_list);
    bool remove_element(size_t element_index);
    bool remove_element(const char * element_name);

public: /* overload */
    bool get_element(const char * element_name, bool & element_value);
    bool get_element(const char * element_name, int8_t & element_value);
    bool get_element(const char * element_name, uint8_t & element_value);
    bool get_element(const char * element_name, int16_t & element_value);
    bool get_element(const char * element_name, uint16_t & element_value);
    bool get_element(const char * element_name, int32_t & element_value);
    bool get_element(const char * element_name, uint32_t & element_value);
    bool get_element(const char * element_name, int64_t & element_value);
    bool get_element(const char * element_name, uint64_t & element_value);
    bool get_element(const char * element_name, float & element_value);
    bool get_element(const char * element_name, double & element_value);

    bool get_element(const char * element_name, std::list<bool> & element_value_list);
    bool get_element(const char * element_name, std::list<int8_t> & element_value_list);
    bool get_element(const char * element_name, std::list<uint8_t> & element_value_list);
    bool get_element(const char * element_name, std::list<int16_t> & element_value_list);
    bool get_element(const char * element_name, std::list<uint16_t> & element_value_list);
    bool get_element(const char * element_name, std::list<int32_t> & element_value_list);
    bool get_element(const char * element_name, std::list<uint32_t> & element_value_list);
    bool get_element(const char * element_name, std::list<int64_t> & element_value_list);
    bool get_element(const char * element_name, std::list<uint64_t> & element_value_list);
    bool get_element(const char * element_name, std::list<float> & element_value_list);
    bool get_element(const char * element_name, std::list<double> & element_value_list);

    bool add_element(const char * element_name, bool element_value, bool as_string);
    bool add_element(const char * element_name, int8_t element_value, bool as_string);
    bool add_element(const char * element_name, uint8_t element_value, bool as_string);
    bool add_element(const char * element_name, int16_t element_value, bool as_string);
    bool add_element(const char * element_name, uint16_t element_value, bool as_string);
    bool add_element(const char * element_name, int32_t element_value, bool as_string);
    bool add_element(const char * element_name, uint32_t element_value, bool as_string);
    bool add_element(const char * element_name, int64_t element_value, bool as_string);
    bool add_element(const char * element_name, uint64_t element_value, bool as_string);
    bool add_element(const char * element_name, float element_value, bool as_string);
    bool add_element(const char * element_name, double element_value, bool as_string);

    bool add_element(const char * element_name, const std::list<bool> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<int8_t> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<uint8_t> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<int16_t> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<uint16_t> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<int32_t> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<uint32_t> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<int64_t> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<uint64_t> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<float> & element_value_list, bool as_string);
    bool add_element(const char * element_name, const std::list<double> & element_value_list, bool as_string);

    bool set_element(const char * element_name, bool element_value, bool as_string);
    bool set_element(const char * element_name, int8_t element_value, bool as_string);
    bool set_element(const char * element_name, uint8_t element_value, bool as_string);
    bool set_element(const char * element_name, int16_t element_value, bool as_string);
    bool set_element(const char * element_name, uint16_t element_value, bool as_string);
    bool set_element(const char * element_name, int32_t element_value, bool as_string);
    bool set_element(const char * element_name, uint32_t element_value, bool as_string);
    bool set_element(const char * element_name, int64_t element_value, bool as_string);
    bool set_element(const char * element_name, uint64_t element_value, bool as_string);
    bool set_element(const char * element_name, float element_value, bool as_string);
    bool set_element(const char * element_name, double element_value, bool as_string);

    bool set_element(const char * element_name, const std::list<bool> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<int8_t> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<uint8_t> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<int16_t> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<uint16_t> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<int32_t> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<uint32_t> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<int64_t> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<uint64_t> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<float> & element_value_list, bool as_string);
    bool set_element(const char * element_name, const std::list<double> & element_value_list, bool as_string);

private: /* template */
    template <typename T> bool do_get_element(const char * element_name, T & element_value);
    template <typename T> bool do_get_element(const char * element_name, std::list<T> & element_value_list);
    template <typename T> bool do_add_element(const char * element_name, T element_value);
    template <typename T> bool do_add_element(const char * element_name, const std::list<T> & element_value_list);
    template <typename T> bool do_set_element(const char * element_name, T element_value);
    template <typename T> bool do_set_element(const char * element_name, const std::list<T> & element_value_list);

private:
    Json::Reader                    m_reader;
    Json::Value                     m_root_value;
    std::list<Json::Value *>        m_child_values;
};

JsonImpl::JsonImpl()
    : m_reader()
    , m_root_value()
    , m_child_values()
{

}

JsonImpl::~JsonImpl()
{

}

bool JsonImpl::into_element(size_t element_index)
{
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isArray() || element_index >= json_parent.size())
    {
        return (false);
    }
    Json::Value & json_child = json_parent[static_cast<Json::Value::ArrayIndex>(element_index)];
    m_child_values.push_back(&json_child);
    return (true);
}

bool JsonImpl::into_element(const char * element_name)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isMember(element_name))
    {
        return (false);
    }
    Json::Value & json_child = json_parent[element_name];
    m_child_values.push_back(&json_child);
    return (true);
}

bool JsonImpl::outof_element()
{
    if (m_child_values.empty())
    {
        return (false);
    }
    m_child_values.pop_back();
    return (true);
}

Goofer::json_value_t::v_t JsonImpl::get_type()
{
    const Json::Value & json_value = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (json_value.isArray())
    {
        return (Goofer::json_value_t::json_array);
    }
    else if (json_value.isObject())
    {
        return (Goofer::json_value_t::json_object);
    }
    else
    {
        return (Goofer::json_value_t::json_scalar);
    }
}

size_t JsonImpl::get_size()
{
    const Json::Value & json_value = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (json_value.isArray())
    {
        return (json_value.size());
    }
    return (0);
}

bool JsonImpl::load(const char * file_name)
{
    if (nullptr == file_name)
    {
        return (false);
    }
    const std::string filename = Goofer::utf8_to_ansi(file_name);
    std::ifstream ifs(filename.c_str(), std::ios::binary);
    if (!ifs.is_open())
    {
        return (false);
    }
    m_root_value.clear();
    m_child_values.clear();
    return (m_reader.parse(ifs, m_root_value, false));
}

bool JsonImpl::set_document(const char * document)
{
    if (nullptr == document)
    {
        return (false);
    }
    m_root_value.clear();
    m_child_values.clear();
    return (m_reader.parse(document, m_root_value, false));
}

bool JsonImpl::find_element(const char * element_name)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    const Json::Value & json_value = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    return (json_value.isMember(element_name));
}

bool JsonImpl::get_element(const char * element_name, std::string & element_value)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    const Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isMember(element_name))
    {
        return (false);
    }
    const Json::Value & json_child = json_parent[element_name];
    if (!json_child.isConvertibleTo(Json::stringValue))
    {
        return (false);
    }
    element_value = json_child.asString();
    return (true);
}

bool JsonImpl::get_element(const char * element_name, char * element_value, size_t element_value_size)
{
    std::string str_element_value;
    if (!get_element(element_name, str_element_value))
    {
        return (false);
    }
    if (nullptr == element_value || str_element_value.size() >= element_value_size)
    {
        return (false);
    }
    strncpy(element_value, str_element_value.c_str(), element_value_size);
    return (true);
}

bool JsonImpl::get_element(const char * element_name, std::list<std::string> & element_value_list)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    const Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isMember(element_name))
    {
        return (false);
    }
    const Json::Value & json_child = json_parent[element_name];
    if (!json_child.isArray())
    {
        return (false);
    }
    for (size_t index = 0; index < json_child.size(); ++index)
    {
        const Json::Value & json_grand_child = json_child[static_cast<Json::Value::ArrayIndex>(index)];
        if (!json_grand_child.isConvertibleTo(Json::stringValue))
        {
            return (false);
        }
        element_value_list.push_back(json_grand_child.asString());
    }
    return (true);
}

bool JsonImpl::save(const char * file_name, bool format)
{
    if (nullptr == file_name)
    {
        return (false);
    }
    std::string document;
    if (!get_document(document, format))
    {
        return (false);
    }
    const std::string filename = Goofer::utf8_to_ansi(file_name);
    std::ofstream ofs(filename.c_str(), std::ios::binary | std::ios::trunc);
    if (!ofs.is_open())
    {
        return (false);
    }
    ofs.write(document.c_str(), document.size());
    ofs.close();
    return (true);
}

bool JsonImpl::save_sub_document(const char * element_name, const char * file_name, bool format)
{
    if (nullptr == file_name)
    {
        return (false);
    }
    std::string sub_document;
    if (!get_sub_document(element_name, sub_document, format))
    {
        return (false);
    }
    const std::string filename = Goofer::utf8_to_ansi(file_name);
    std::ofstream ofs(filename.c_str(), std::ios::binary | std::ios::trunc);
    if (!ofs.is_open())
    {
        return (false);
    }
    ofs.write(sub_document.c_str(), sub_document.size());
    ofs.close();
    return (true);
}

bool JsonImpl::save_sub_document(size_t element_index, const char * file_name, bool format)
{
    if (nullptr == file_name)
    {
        return (false);
    }
    std::string sub_document;
    if (!get_sub_document(element_index, sub_document, format))
    {
        return (false);
    }
    const std::string filename = Goofer::utf8_to_ansi(file_name);
    std::ofstream ofs(filename.c_str(), std::ios::binary | std::ios::trunc);
    if (!ofs.is_open())
    {
        return (false);
    }
    ofs.write(sub_document.c_str(), sub_document.size());
    ofs.close();
    return (true);
}

bool JsonImpl::get_document(std::string & document, bool format)
{
    Json::FastWriter fast_writer;
    Json::StyledWriter styled_writer;
    document = (format ? styled_writer.write(m_root_value) : fast_writer.write(m_root_value));
    return (true);
}

bool JsonImpl::get_sub_document(const char * element_name, std::string & sub_document, bool format)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    const Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isMember(element_name))
    {
        return (false);
    }
    const Json::Value & json_child = json_parent[element_name];
    Json::FastWriter fast_writer;
    Json::StyledWriter styled_writer;
    sub_document = (format ? styled_writer.write(json_child) : fast_writer.write(json_child));
    return (true);
}

bool JsonImpl::get_sub_document(size_t element_index, std::string & sub_document, bool format)
{
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isArray() || element_index >= json_parent.size())
    {
        return (false);
    }
    const Json::Value & json_child = json_parent[static_cast<Json::Value::ArrayIndex>(element_index)];
    Json::FastWriter fast_writer;
    Json::StyledWriter styled_writer;
    sub_document = (format ? styled_writer.write(json_child) : fast_writer.write(json_child));
    return (true);
}

bool JsonImpl::set_sub_document(const char * element_name, const char * sub_document)
{
    if (nullptr == sub_document)
    {
        return (false);
    }
    Json::Reader reader;
    Json::Value json_child;
    if (!reader.parse(sub_document, json_child, false))
    {
        return (false);
    }
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    json_parent[element_name] = json_child;
    return (true);
}

bool JsonImpl::set_sub_document(size_t element_index, const char * sub_document)
{
    if (nullptr == sub_document)
    {
        return (false);
    }
    Json::Reader reader;
    Json::Value json_child;
    if (!reader.parse(sub_document, json_child, false))
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isNull() && !json_parent.isArray())
    {
        return (false);
    }
    if (element_index > json_parent.size())
    {
        return (false);
    }
    json_parent[static_cast<Json::Value::ArrayIndex>(element_index)] = json_child;
    return (true);
}

bool JsonImpl::add_array(const char * element_name)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    Json::Value & json_child = json_parent[element_name];
    json_child.resize(0);
    return (true);
}

bool JsonImpl::add_element(size_t element_index)
{
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isNull() && !json_parent.isArray())
    {
        return (false);
    }
    if (element_index > json_parent.size())
    {
        return (false);
    }
    const Json::Value & json_child = json_parent[static_cast<Json::Value::ArrayIndex>(element_index)];
    return (true);
}

bool JsonImpl::add_element(const char * element_name)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    const Json::Value & json_child = json_parent[element_name];
    return (true);
}

bool JsonImpl::add_element(const char * element_name, const char * element_value)
{
    if (nullptr == element_name || nullptr == element_value)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    Json::Value & json_child = json_parent[element_name];
    json_child = element_value;
    return (true);
}

bool JsonImpl::add_element(const char * element_name, const std::string & element_value)
{
    return (add_element(element_name, element_value.c_str()));
}

bool JsonImpl::add_element(const char * element_name, const std::list<std::string> & element_value_list)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    Json::Value & json_child = json_parent[element_name];
    for (std::list<std::string>::const_iterator iter = element_value_list.begin(); element_value_list.end() != iter; ++iter)
    {
        json_child.append(*iter);
    }
    return (true);
}

bool JsonImpl::set_element(const char * element_name, const char * element_value)
{
    if (nullptr == element_name || nullptr == element_value)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    Json::Value & json_child = json_parent[element_name];
    json_child = element_value;
    return (true);
}

bool JsonImpl::set_element(const char * element_name, const std::string & element_value)
{
    return (set_element(element_name, element_value.c_str()));
}

bool JsonImpl::set_element(const char * element_name, const std::list<std::string> & element_value_list)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    Json::Value & json_child = json_parent[element_name];
    json_child.clear();
    for (std::list<std::string>::const_iterator iter = element_value_list.begin(); element_value_list.end() != iter; ++iter)
    {
        json_child.append(*iter);
    }
    return (true);
}

bool JsonImpl::remove_element(size_t element_index)
{
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isArray() || element_index >= json_parent.size())
    {
        return (false);
    }
    Json::Value json_child;
    return (json_parent.removeIndex(static_cast<Json::Value::ArrayIndex>(element_index), &json_child));
}

bool JsonImpl::remove_element(const char * element_name)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    if (!json_parent.isMember(element_name))
    {
        return (false);
    }
    Json::Value json_child;
    return (json_parent.removeMember(element_name, &json_child));
}

bool JsonImpl::get_element(const char * element_name, bool & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, int8_t & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, uint8_t & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, int16_t & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, uint16_t & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, int32_t & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, uint32_t & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, int64_t & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, uint64_t & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, float & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, double & element_value)
{
    return (do_get_element(element_name, element_value));
}

bool JsonImpl::get_element(const char * element_name, std::list<bool> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<int8_t> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<uint8_t> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<int16_t> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<uint16_t> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<int32_t> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<uint32_t> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<int64_t> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<uint64_t> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<float> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::get_element(const char * element_name, std::list<double> & element_value_list)
{
    return (do_get_element(element_name, element_value_list));
}

bool JsonImpl::add_element(const char * element_name, bool element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, int8_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, uint8_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, int16_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, uint16_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, int32_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, uint32_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, int64_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, uint64_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, float element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, double element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && add_element(element_name, str_element_value));
    }
    else
    {
        return (do_add_element(element_name, element_value));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<bool> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<int8_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<uint8_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<int16_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<uint16_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<int32_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<uint32_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<int64_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<uint64_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<float> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::add_element(const char * element_name, const std::list<double> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && add_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_add_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, bool element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, int8_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, uint8_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, int16_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, uint16_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, int32_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, uint32_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, int64_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, uint64_t element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, float element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, double element_value, bool as_string)
{
    if (as_string)
    {
        std::string str_element_value;
        return (Goofer::goofer_type_to_string(element_value, str_element_value) && set_element(element_name, str_element_value));
    }
    else
    {
        return (do_set_element(element_name, element_value));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<bool> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<int8_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<uint8_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<int16_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<uint16_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<int32_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<uint32_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<int64_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<uint64_t> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<float> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

bool JsonImpl::set_element(const char * element_name, const std::list<double> & element_value_list, bool as_string)
{
    if (as_string)
    {
        std::list<std::string> str_element_value_list;
        return (Goofer::goofer_type_to_string(element_value_list, str_element_value_list) && set_element(element_name, str_element_value_list));
    }
    else
    {
        return (do_set_element(element_name, element_value_list));
    }
}

template <typename T>
bool JsonImpl::do_get_element(const char * element_name, T & element_value)
{
    std::string str_element_value;
    return (get_element(element_name, str_element_value) && Goofer::goofer_string_to_type(str_element_value, element_value));
}

template <typename T>
bool JsonImpl::do_get_element(const char * element_name, std::list<T> & element_value_list)
{
    std::list<std::string> str_element_value_list;
    return (get_element(element_name, str_element_value_list) && Goofer::goofer_string_to_type(str_element_value_list, element_value_list));
}

template <typename T>
bool JsonImpl::do_add_element(const char * element_name, T element_value)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    Json::Value & json_child = json_parent[element_name];
    json_child = element_value;
    return (true);
}

template <typename T>
bool JsonImpl::do_add_element(const char * element_name, const std::list<T> & element_value_list)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    Json::Value & json_child = json_parent[element_name];
    if (element_value_list.empty())
    {
        json_child.resize(0);
    }
    else
    {
        for (typename std::list<T>::const_iterator iter = element_value_list.begin(); element_value_list.end() != iter; ++iter)
        {
            json_child.append(*iter);
        }
    }
    return (true);
}

template <typename T>
bool JsonImpl::do_set_element(const char * element_name, T element_value)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    Json::Value & json_child = json_parent[element_name];
    json_child = element_value;
    return (true);
}

template <typename T>
bool JsonImpl::do_set_element(const char * element_name, const std::list<T> & element_value_list)
{
    if (nullptr == element_name)
    {
        return (false);
    }
    Json::Value & json_parent = (m_child_values.empty() ? m_root_value : *m_child_values.back());
    Json::Value & json_child = json_parent[element_name];
    if (element_value_list.empty())
    {
        json_child.resize(0);
    }
    else
    {
        json_child.clear();
        for (typename std::list<T>::const_iterator iter = element_value_list.begin(); element_value_list.end() != iter; ++iter)
        {
            json_child.append(*iter);
        }
    }
    return (true);
}

NAMESPACE_GOOFER_BEGIN

Json::Json() : m_json_impl(new JsonImpl)
{
    assert(nullptr != m_json_impl);
}

Json::~Json()
{
    delete m_json_impl;
}

bool Json::into_element(size_t element_index)
{
    return (nullptr != m_json_impl && m_json_impl->into_element(element_index));
}

bool Json::into_element(const char * element_name)
{
    return (nullptr != m_json_impl && m_json_impl->into_element(element_name));
}

bool Json::outof_element()
{
    return (nullptr != m_json_impl && m_json_impl->outof_element());
}

json_value_t::v_t Json::get_type()
{
    return (nullptr != m_json_impl ? m_json_impl->get_type() : json_value_t::json_scalar);
}

size_t Json::get_size()
{
    return (nullptr != m_json_impl ? m_json_impl->get_size() : 0);
}

bool Json::load(const char * file_name)
{
    return (nullptr != m_json_impl && m_json_impl->load(file_name));
}

bool Json::set_document(const char * document)
{
    return (nullptr != m_json_impl && m_json_impl->set_document(document));
}

bool Json::find_element(const char * element_name)
{
    return (nullptr != m_json_impl && m_json_impl->find_element(element_name));
}

bool Json::get_element(const char * element_name, std::string & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, char * element_value, size_t element_value_size)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value, element_value_size));
}

bool Json::get_element(const char * element_name, std::list<std::string> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::save(const char * file_name, bool format)
{
    return (nullptr != m_json_impl && m_json_impl->save(file_name, format));
}

bool Json::save_sub_document(const char * element_name, const char * file_name, bool format)
{
    return (nullptr != m_json_impl && m_json_impl->save_sub_document(element_name, file_name, format));
}

bool Json::save_sub_document(size_t element_index, const char * file_name, bool format)
{
    return (nullptr != m_json_impl && m_json_impl->save_sub_document(element_index, file_name, format));
}

bool Json::set_sub_document(const char * element_name, const char * sub_document)
{
    return (nullptr != m_json_impl && m_json_impl->set_sub_document(element_name, sub_document));
}

bool Json::set_sub_document(size_t element_index, const char * sub_document)
{
    return (nullptr != m_json_impl && m_json_impl->set_sub_document(element_index, sub_document));
}

bool Json::get_document(std::string & document, bool format)
{
    return (nullptr != m_json_impl && m_json_impl->get_document(document, format));
}

bool Json::get_sub_document(const char * element_name, std::string & sub_document, bool format)
{
    return (nullptr != m_json_impl && m_json_impl->get_sub_document(element_name, sub_document, format));
}

bool Json::get_sub_document(size_t element_index, std::string & sub_document, bool format)
{
    return (nullptr != m_json_impl && m_json_impl->get_sub_document(element_index, sub_document, format));
}

bool Json::add_array(const char * element_name)
{
    return (nullptr != m_json_impl && m_json_impl->add_array(element_name));
}

bool Json::add_element(size_t element_index)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_index));
}

bool Json::add_element(const char * element_name)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name));
}

bool Json::add_element(const char * element_name, const char * element_value)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value));
}

bool Json::add_element(const char * element_name, const std::string & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value));
}

bool Json::add_element(const char * element_name, const std::list<std::string> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list));
}

bool Json::set_element(const char * element_name, const char * element_value)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value));
}

bool Json::set_element(const char * element_name, const std::string & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value));
}

bool Json::set_element(const char * element_name, const std::list<std::string> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list));
}

bool Json::remove_element(size_t element_index)
{
    return (nullptr != m_json_impl && m_json_impl->remove_element(element_index));
}

bool Json::remove_element(const char * element_name)
{
    return (nullptr != m_json_impl && m_json_impl->remove_element(element_name));
}

bool Json::get_element(const char * element_name, bool & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, int8_t & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, uint8_t & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, int16_t & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, uint16_t & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, int32_t & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, uint32_t & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, int64_t & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, uint64_t & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, float & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, double & element_value)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value));
}

bool Json::get_element(const char * element_name, std::list<bool> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<int8_t> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<uint8_t> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<int16_t> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<uint16_t> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<int32_t> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<uint32_t> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<int64_t> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<uint64_t> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<float> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::get_element(const char * element_name, std::list<double> & element_value_list)
{
    return (nullptr != m_json_impl && m_json_impl->get_element(element_name, element_value_list));
}

bool Json::add_element(const char * element_name, bool element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, int8_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, uint8_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, int16_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, uint16_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, int32_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, uint32_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, int64_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, uint64_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, float element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, double element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value, as_string));
}

bool Json::add_element(const char * element_name, const std::list<bool> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<int8_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<uint8_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<int16_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<uint16_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<int32_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<uint32_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<int64_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<uint64_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<float> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::add_element(const char * element_name, const std::list<double> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->add_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, bool element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, int8_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, uint8_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, int16_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, uint16_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, int32_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, uint32_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, int64_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, uint64_t element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, float element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, double element_value, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value, as_string));
}

bool Json::set_element(const char * element_name, const std::list<bool> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<int8_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<uint8_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<int16_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<uint16_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<int32_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<uint32_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<int64_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<uint64_t> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<float> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

bool Json::set_element(const char * element_name, const std::list<double> & element_value_list, bool as_string)
{
    return (nullptr != m_json_impl && m_json_impl->set_element(element_name, element_value_list, as_string));
}

NAMESPACE_GOOFER_END
