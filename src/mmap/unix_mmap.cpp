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

#ifndef _MSC_VER


#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "charset/charset.h"
#include "mmap/unix_mmap.h"

NAMESPACE_GOOFER_BEGIN

UnixVirtualMemoryMap::UnixVirtualMemoryMap()
    : m_filename()
    , m_is_file(false)
    , m_is_creator(false)
    , m_map_fd(-1)
    , m_map_data(reinterpret_cast<char *>(-1))
    , m_map_size(0)
    , m_map_capacity(0)
{

}

UnixVirtualMemoryMap::~UnixVirtualMemoryMap()
{
    exit();
}

bool UnixVirtualMemoryMap::init(const char * filename, std::size_t map_size, bool file, bool creator, bool/*global*/)
{
    exit();

    do
    {
        if (nullptr == filename)
        {
            break;
        }

        m_filename = utf8_to_ansi(filename);

        if (file)
        {
            m_map_fd = ::open(m_filename.c_str(), creator ? O_RDWR | O_CREAT : O_RDWR, 0644);
        }
        else
        {
            m_map_fd = ::shm_open(m_filename.c_str(), creator ? O_RDWR | O_CREAT : O_RDWR, 0644);
        }

        if (m_map_fd < 0)
        {
            break;
        }

        m_is_file = file;
        m_is_creator = creator;

        struct stat filestat = { 0x0 };
        if (::fstat(m_map_fd, &filestat) < 0)
        {
            break;
        }

        m_map_capacity = static_cast<std::size_t>(filestat.st_size);

        if (map_size > m_map_capacity)
        {
            if (::ftruncate(m_map_fd, static_cast<off_t>(map_size)) < 0)
            {
                break;
            }
            m_map_capacity = map_size;
        }
        else if (map_size < m_map_capacity)
        {
            if (file)
            {
                if (0 == map_size)
                {
                    map_size = m_map_capacity;
                }
            }
            else
            {
                if (::ftruncate(m_map_fd, static_cast<off_t>(map_size)) < 0)
                {
                    break;
                }
                m_map_capacity = map_size;
            }
        }

        if (0 == map_size)
        {
            break;
        }

        m_map_data = reinterpret_cast<char *>(::mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_map_fd, 0));
        if (reinterpret_cast<char *>(-1) == m_map_data)
        {
            break;
        }

        m_map_size = map_size;

        return (true);
    } while (false);

    exit();

    return (false);
}

void UnixVirtualMemoryMap::exit()
{
    if (m_map_fd < 0)
    {
        return;
    }

    if (reinterpret_cast<void *>(-1) != m_map_data)
    {
        ::munmap(m_map_data, m_map_size);
    }

    ::close(m_map_fd);

    if (!m_is_file && m_is_creator)
    {
    //  ::shm_unlink(m_filename.c_str());
    }

    m_filename.clear();
    m_is_file = false;
    m_is_creator = false;
    m_map_fd = -1;
    m_map_data = reinterpret_cast<char *>(-1);
    m_map_size = 0;
    m_map_capacity = 0;
}

bool UnixVirtualMemoryMap::resize(std::size_t map_size)
{
    if (m_map_fd < 0)
    {
        return (false);
    }

    if (map_size == m_map_capacity)
    {
        return (true);
    }

    do
    {
        if (::munmap(m_map_data, m_map_size) < 0)
        {
            break;
        }

        if (map_size > m_map_capacity)
        {
            if (::ftruncate(m_map_fd, static_cast<off_t>(map_size)) < 0)
            {
                break;
            }
            m_map_capacity = map_size;
        }
        else if (map_size < m_map_capacity)
        {
            if (m_map_fd)
            {
                if (0 == map_size)
                {
                    map_size = m_map_capacity;
                }
            }
            else
            {
                if (::ftruncate(m_map_fd, static_cast<off_t>(map_size)) < 0)
                {
                    break;
                }
                m_map_capacity = map_size;
            }
        }

        if (0 == map_size)
        {
            break;
        }

        m_map_data = reinterpret_cast<char *>(::mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_map_fd, 0));
        if (reinterpret_cast<char *>(-1) == m_map_data)
        {
            break;
        }

        m_map_size = map_size;

        return (true);
    } while (false);

    exit();

    return (false);
}

char * UnixVirtualMemoryMap::get_map_data()
{
    return (m_map_data);
}

std::size_t UnixVirtualMemoryMap::get_map_size()
{
    return (m_map_size);
}

UnixMemMap::UnixMemMap()
    : UnixVirtualMemoryMap()
{

}

UnixMemMap::~UnixMemMap()
{

}

bool UnixMemMap::init(const char * map_name, bool creator, std::size_t map_size, bool global)
{
    return (UnixVirtualMemoryMap::init(map_name, map_size, false, creator, global));
}

UnixFileMap::UnixFileMap()
    : UnixVirtualMemoryMap()
{

}

UnixFileMap::~UnixFileMap()
{

}

bool UnixFileMap::init(const char * file_name, bool creator, std::size_t map_size, bool global)
{
    return (UnixVirtualMemoryMap::init(file_name, map_size, true, creator, global));
}

bool UnixFileMap::flush(char * data, std::size_t size)
{
    if (m_map_fd < 0 || nullptr == m_map_data || 0 == m_map_size)
    {
        return (false);
    }

    if (nullptr == data || 0 == size)
    {
        data = m_map_data;
        size = m_map_size;
    }
    else if (data < m_map_data || data + size > m_map_data + m_map_size)
    {
        return (false);
    }

    if (::msync(data, size, MS_ASYNC) < 0)
    {
        return (false);
    }

    return (true);
}

NAMESPACE_GOOFER_END


#endif // _MSC_VER
