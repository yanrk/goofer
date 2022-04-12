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

#ifdef _MSC_VER


#include <algorithm>
#include "filesystem/directory.h"
#include "filesystem/file.h"
#include "charset/charset.h"
#include "mmap/windows_mmap.h"

static std::wstring get_file_map_name(HANDLE file_handle)
{
    std::wstring map_name;

    if (INVALID_HANDLE_VALUE == file_handle)
    {
        return (map_name);
    }

    wchar_t path_name[MAX_PATH] = { 0x0 };
    if (0 == GetFinalPathNameByHandleW(file_handle, path_name, sizeof(path_name) / sizeof(path_name[0]), VOLUME_NAME_DOS))
    {
        return (map_name);
    }

    map_name = path_name;
    for (std::wstring::iterator iter = map_name.begin(); map_name.end() != iter; ++iter)
    {
        wchar_t & map_part = *iter;
        if (L'\\' == map_part)
        {
            map_part = L'/';
        }
        else if (L'A' <= map_part && L'Z' >= map_part)
        {
            map_part = L'a' + (map_part - L'A');
        }
    }

    return (map_name);
}

static bool truncate_file(HANDLE file_handle, std::size_t file_length)
{
    if (INVALID_HANDLE_VALUE == file_handle)
    {
        return (false);
    }

    LARGE_INTEGER file_size;
    file_size.QuadPart = static_cast<LONGLONG>(file_length);
    if (!::SetFilePointerEx(file_handle, file_size, nullptr, FILE_BEGIN))
    {
        return (false);
    }

    if (!::SetEndOfFile(file_handle))
    {
        return (false);
    }

    return (true);
}

NAMESPACE_GOOFER_BEGIN

WindowsVirtualMemoryMap::WindowsVirtualMemoryMap()
    : m_is_file(false)
    , m_is_creator(false)
    , m_file_handle(INVALID_HANDLE_VALUE)
    , m_map_name()
    , m_map_handle(nullptr)
    , m_map_data(nullptr)
    , m_map_size(0)
    , m_map_capacity(0)
{

}

WindowsVirtualMemoryMap::~WindowsVirtualMemoryMap()
{
    exit();
}

bool WindowsVirtualMemoryMap::init(const char * filename, std::size_t map_size, bool file, bool creator, bool global)
{
    exit();

    do
    {
        if (nullptr == filename)
        {
            break;
        }

        if (file)
        {
            m_file_handle = ::CreateFileW(utf8_to_unicode(filename).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, creator ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (INVALID_HANDLE_VALUE == m_file_handle)
            {
                break;
            }

            m_map_name = (global ? L"Global\\mmap_" : L"Local\\mmap_") + get_file_map_name(m_file_handle);

            goofer_stat_t filestat = { 0x0 };
            if (!goofer_stat_safe(filename, filestat))
            {
                break;
            }

            m_map_capacity = static_cast<std::size_t>(filestat.st_size);

            if (map_size > m_map_capacity)
            {
                if (!::truncate_file(m_file_handle, map_size))
                {
                    break;
                }
                m_map_capacity = map_size;
            }
            else if (map_size < m_map_capacity)
            {
                if (0 == map_size)
                {
                    map_size = m_map_capacity;
                }
            }
        }
        else
        {
            m_map_name = (global ? L"Global\\mmap_" : L"Local\\mmap_") + utf8_to_unicode(filename);
            m_map_capacity = map_size;
        }

        if (0 == map_size)
        {
            break;
        }

        m_is_file = file;
        m_is_creator = creator;

        if (creator)
        {
            LARGE_INTEGER map_length;
            map_length.QuadPart = static_cast<LONGLONG>(map_size);
            m_map_handle = ::CreateFileMappingW(m_file_handle, nullptr, PAGE_READWRITE, map_length.HighPart, map_length.LowPart, m_map_name.c_str());
        }
        else
        {
            m_map_handle = ::OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, m_map_name.c_str());
        }

        if (nullptr == m_map_handle)
        {
            break;
        }

        m_map_data = reinterpret_cast<char *>(::MapViewOfFile(m_map_handle, FILE_MAP_ALL_ACCESS, 0, 0, 0));
        if (nullptr == m_map_data)
        {
            break;
        }

        m_map_size = map_size;

        return (true);
    } while (false);

    exit();

    return (false);
}

void WindowsVirtualMemoryMap::exit()
{
    if (INVALID_HANDLE_VALUE == m_file_handle)
    {
        return;
    }

    if (nullptr != m_map_data)
    {
        ::UnmapViewOfFile(m_map_data);
    }

    if (nullptr != m_map_handle)
    {
        ::CloseHandle(m_map_handle);
    }

    ::CloseHandle(m_file_handle);

    m_is_file = false;
    m_is_creator = false;
    m_file_handle = INVALID_HANDLE_VALUE;
    m_map_name.clear();
    m_map_handle = nullptr;
    m_map_data = nullptr;
    m_map_size = 0;
    m_map_capacity = 0;
}

bool WindowsVirtualMemoryMap::resize(std::size_t map_size)
{
    if (INVALID_HANDLE_VALUE == m_file_handle)
    {
        return (false);
    }

    if (map_size == m_map_capacity)
    {
        return (true);
    }

    do
    {
        if (nullptr != m_map_data)
        {
            if (!::UnmapViewOfFile(m_map_data))
            {
                break;
            }
            m_map_data = nullptr;
        }

        if (nullptr != m_map_handle)
        {
            ::CloseHandle(m_map_handle);
            m_map_handle = nullptr;
        }

        if (m_is_file)
        {
            if (map_size > m_map_capacity)
            {
                if (!::truncate_file(m_file_handle, map_size))
                {
                    break;
                }
                m_map_capacity = map_size;
            }
            else if (map_size < m_map_capacity)
            {
                if (0 == map_size)
                {
                    map_size = m_map_capacity;
                }
            }
        }
        else
        {
            m_map_capacity = map_size;
        }

        if (0 == map_size)
        {
            break;
        }

        if (m_is_creator)
        {
            LARGE_INTEGER map_length;
            map_length.QuadPart = static_cast<LONGLONG>(map_size);
            m_map_handle = ::CreateFileMappingW(m_file_handle, nullptr, PAGE_READWRITE, map_length.HighPart, map_length.LowPart, m_map_name.c_str());
        }
        else
        {
            m_map_handle = ::OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, m_map_name.c_str());
        }

        if (nullptr == m_map_handle)
        {
            break;
        }

        m_map_data = reinterpret_cast<char *>(::MapViewOfFile(m_map_handle, FILE_MAP_ALL_ACCESS, 0, 0, 0));
        if (nullptr == m_map_data)
        {
            break;
        }

        m_map_size = map_size;

        return (true);
    } while (false);

    exit();

    return (false);
}

char * WindowsVirtualMemoryMap::get_map_data()
{
    return (m_map_data);
}

std::size_t WindowsVirtualMemoryMap::get_map_size()
{
    return (m_map_size);
}

WindowsMemMap::WindowsMemMap()
    : WindowsVirtualMemoryMap()
{

}

WindowsMemMap::~WindowsMemMap()
{

}

bool WindowsMemMap::init(const char * map_name, bool creator, std::size_t map_size, bool global)
{
    return (WindowsVirtualMemoryMap::init(map_name, map_size, false, creator, global));
}

WindowsFileMap::WindowsFileMap()
    : WindowsVirtualMemoryMap()
{

}

WindowsFileMap::~WindowsFileMap()
{

}

bool WindowsFileMap::init(const char * file_name, bool creator, std::size_t map_size, bool global)
{
    return (WindowsVirtualMemoryMap::init(file_name, map_size, true, creator, global));
}

bool WindowsFileMap::flush(char * data, std::size_t size)
{
    if (INVALID_HANDLE_VALUE == m_file_handle || nullptr == m_map_data || 0 == m_map_size)
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

    if (!::FlushViewOfFile(data, size))
    {
        return (false);
    }

    return (true);
}

NAMESPACE_GOOFER_END


#endif // _MSC_VER
