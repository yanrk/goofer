/********************************************************
 * Description : virtual memory map class in unix
 * Date        : 2022-01-25 12:11:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2022
 ********************************************************/

#ifndef GOOFER_UNIX_VIRTUAL_MEMORY_MAP_H
#define GOOFER_UNIX_VIRTUAL_MEMORY_MAP_H


#ifndef _MSC_VER


#include <string>
#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API UnixVirtualMemoryMap : private Uncopy
{
public:
    explicit UnixVirtualMemoryMap();
    ~UnixVirtualMemoryMap();

public:
    bool init(const char * filename, std::size_t map_size, bool file, bool creator, bool global);
    void exit();
    bool resize(std::size_t map_size);

public:
    char * get_map_data();
    std::size_t get_map_size();

protected:
    std::string                 m_filename;
    bool                        m_is_file;
    bool                        m_is_creator;
    int                         m_map_fd;
    char                      * m_map_data;
    std::size_t                 m_map_size;
    std::size_t                 m_map_capacity;
};

class GOOFER_API UnixMemMap : public UnixVirtualMemoryMap
{
public:
    explicit UnixMemMap();
    ~UnixMemMap();

public:
    bool init(const char * map_name, bool creator, std::size_t map_size, bool global = false);
};

class GOOFER_API UnixFileMap : public UnixVirtualMemoryMap
{
public:
    explicit UnixFileMap();
    ~UnixFileMap();

public:
    bool init(const char * file_name, bool creator, std::size_t map_size = 0, bool global = false);
    bool flush(char * data = nullptr, std::size_t size = 0);
};

NAMESPACE_GOOFER_END


#endif // _MSC_VER


#endif // GOOFER_UNIX_VIRTUAL_MEMORY_MAP_H
