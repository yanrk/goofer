/********************************************************
 * Description : virtual memory map class in windows
 * Date        : 2022-01-25 18:17:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2022
 ********************************************************/

#ifndef GOOFER_VIRTUAL_WINDOWS_MEMORY_MAP_H
#define GOOFER_VIRTUAL_WINDOWS_MEMORY_MAP_H


#ifdef _MSC_VER


#include <windows.h>

#include <string>
#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API WindowsVirtualMemoryMap : private Uncopy
{
public:
    explicit WindowsVirtualMemoryMap();
    ~WindowsVirtualMemoryMap();

public:
    bool init(const char * filename, std::size_t map_size, bool file, bool creator, bool global);
    void exit();
    bool resize(std::size_t map_size);

public:
    char * get_map_data();
    std::size_t get_map_size();

protected:
    bool                        m_is_file;
    bool                        m_is_creator;
    HANDLE                      m_file_handle;
    std::wstring                m_map_name;
    HANDLE                      m_map_handle;
    char                      * m_map_data;
    std::size_t                 m_map_size;
    std::size_t                 m_map_capacity;
};

class GOOFER_API WindowsMemMap : public WindowsVirtualMemoryMap
{
public:
    explicit WindowsMemMap();
    ~WindowsMemMap();

public:
    bool init(const char * map_name, bool creator, std::size_t map_size, bool global = false);
};

class GOOFER_API WindowsFileMap : public WindowsVirtualMemoryMap
{
public:
    explicit WindowsFileMap();
    ~WindowsFileMap();

public:
    bool init(const char * file_name, bool creator, std::size_t map_size = 0, bool global = false);
    bool flush(char * data = nullptr, std::size_t size = 0);
};

NAMESPACE_GOOFER_END


#endif // _MSC_VER


#endif // GOOFER_VIRTUAL_WINDOWS_MEMORY_MAP_H
