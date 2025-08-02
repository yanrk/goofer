/********************************************************
 * Description : file operate class
 * Date        : 2015-08-24 17:21:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include "time/time.h"
#include "filesystem/file.h"
#include "filesystem/sys_io.h"
#include "charset/charset.h"
#include "string/string.h"

#ifdef GOOFER_OS_IS_WIN
    #include <sys/types.h>
    #include <sys/utime.h>
#else
    #include <sys/types.h>
    #include <utime.h>
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_BEGIN

File::File()
    : m_is_write(false)
    , m_is_truncate(false)
    , m_file_name()
#ifdef GOOFER_OS_IS_WIN
    , m_file(INVALID_HANDLE_VALUE)
    , m_is_eof(false)
#else
    , m_file(nullptr)
#endif // GOOFER_OS_IS_WIN
{

}

File::~File()
{
    close();
}

bool File::open(const char * filename, bool is_write, bool is_truncate)
{
    if (nullptr != filename)
    {
        m_is_write = is_write;
        m_is_truncate = is_truncate;
        m_file_name = filename;
    }

#ifdef GOOFER_OS_IS_WIN
    if (INVALID_HANDLE_VALUE != m_file)
    {
        ::CloseHandle(m_file);
        m_file = INVALID_HANDLE_VALUE;
    }

    m_is_eof = false;

    DWORD desired_access = 0;
    DWORD share_mode = FILE_SHARE_READ;
    DWORD creation_disposition = 0;
    DWORD flags_and_attributes = FILE_ATTRIBUTE_NORMAL;
    if (is_write)
    {
        desired_access = GENERIC_WRITE;
        creation_disposition = (is_truncate ? CREATE_ALWAYS : OPEN_ALWAYS);
    }
    else
    {
        desired_access = GENERIC_READ;
        creation_disposition = OPEN_EXISTING;
    }

    m_file = ::CreateFileW(utf8_to_unicode(filename).c_str(), desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, nullptr);

    if (is_write && !m_is_truncate && INVALID_HANDLE_VALUE != m_file)
    {
        LARGE_INTEGER distance = { 0x0 };
        ::SetFilePointerEx(m_file, distance, nullptr, FILE_END);
    }

    return INVALID_HANDLE_VALUE != m_file;
#else
    file_close(m_file);

    if (is_write)
    {
        m_file = file_open_for_write(filename, is_truncate);
    }
    else
    {
        m_file = file_open_for_read(filename);
    }

    return file_is_open(m_file);
#endif // GOOFER_OS_IS_WIN
}

bool File::is_open() const
{
#ifdef GOOFER_OS_IS_WIN
    return INVALID_HANDLE_VALUE != m_file;
#else
    return file_is_open(m_file);
#endif // GOOFER_OS_IS_WIN
}

bool File::is_eof() const
{
#ifdef GOOFER_OS_IS_WIN
    return INVALID_HANDLE_VALUE != m_file && m_is_eof;
#else
    return file_eof(m_file);
#endif // GOOFER_OS_IS_WIN
}

bool File::read(char * buff, size_t buff_size, size_t & read_len)
{
    if (m_is_write)
    {
        read_len = 0;
        return false;
    }

#ifdef GOOFER_OS_IS_WIN
    read_len = 0;
    if (INVALID_HANDLE_VALUE == m_file)
    {
        return false;
    }
    while (buff_size > 0)
    {
        DWORD read_size = 0;
        if (::ReadFile(m_file, buff, static_cast<DWORD>(buff_size), &read_size, nullptr))
        {
            if (0 != read_size)
            {
                buff += read_size;
                buff_size -= read_size;
                read_len += read_size;
            }
            else
            {
                m_is_eof = true;
                break;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
#else
    return file_read(m_file, buff, buff_size, read_len);
#endif // GOOFER_OS_IS_WIN
}

bool File::write(const char * data, size_t data_len)
{
    if (!m_is_write)
    {
        return false;
    }

#ifdef GOOFER_OS_IS_WIN
    if (INVALID_HANDLE_VALUE == m_file)
    {
        return false;
    }
    while (data_len > 0)
    {
        DWORD write_size = 0;
        if (::WriteFile(m_file, data, static_cast<DWORD>(data_len), &write_size, nullptr))
        {
            data += write_size;
            data_len -= write_size;
        }
        else
        {
            return false;
        }
    }
    return true;
#else
    return file_write(m_file, data, data_len);
#endif // GOOFER_OS_IS_WIN
}

bool File::seek(int64_t offset, int whence)
{
    if (m_is_write && !m_is_truncate)
    {
        return false;
    }

#ifdef GOOFER_OS_IS_WIN
    if (INVALID_HANDLE_VALUE == m_file)
    {
        return false;
    }
    LARGE_INTEGER distance = { 0x0 };
    distance.QuadPart = static_cast<LONGLONG>(offset);
    return 0 != ::SetFilePointerEx(m_file, distance, nullptr, whence);
#else
    return file_seek(m_file, offset, whence);
#endif // GOOFER_OS_IS_WIN
}

bool File::seek(int64_t file_pos)
{
    if (m_is_write && !m_is_truncate)
    {
        return false;
    }

#ifdef GOOFER_OS_IS_WIN
    return seek(file_pos, FILE_BEGIN);
#else
    return file_seek(m_file, file_pos);
#endif // GOOFER_OS_IS_WIN
}

bool File::tell(int64_t & file_pos)
{
    if (m_is_write && !m_is_truncate)
    {
        return false;
    }

#ifdef GOOFER_OS_IS_WIN
    if (INVALID_HANDLE_VALUE == m_file)
    {
        return false;
    }
    if (m_is_write)
    {
        ::FlushFileBuffers(m_file);
    }
    LARGE_INTEGER distance = { 0x0 };
    bool ret = (0 != ::SetFilePointerEx(m_file, distance, &distance, FILE_CURRENT));
    file_pos = static_cast<int64_t>(distance.QuadPart);
    return ret;
#else
    return file_tell(m_file, file_pos);
#endif // GOOFER_OS_IS_WIN
}

bool File::flush()
{
#ifdef GOOFER_OS_IS_WIN
    return m_is_write && INVALID_HANDLE_VALUE != m_file && ::FlushFileBuffers(m_file);
#else
    return file_flush(m_file);
#endif // GOOFER_OS_IS_WIN
}

void File::close()
{
#ifdef GOOFER_OS_IS_WIN
    if (INVALID_HANDLE_VALUE != m_file)
    {
        ::CloseHandle(m_file);
        m_file = INVALID_HANDLE_VALUE;
    }
    m_is_eof = false;
#else
    file_close(m_file);
#endif // GOOFER_OS_IS_WIN

    m_is_write = false;
    m_is_truncate = false;
    m_file_name.clear();
}

FILE * file_open_for_read(const char * filename)
{
    if (nullptr == filename || '\0' == filename[0])
    {
        return nullptr;
    }
#ifdef GOOFER_OS_IS_WIN
    const wchar_t * mode = L"rb";
    return _wfopen(utf8_to_unicode(filename).c_str(), mode);
#else
    const char * mode = "rb";
    return fopen(utf8_to_ansi(filename).c_str(), mode);
#endif // GOOFER_OS_IS_WIN
}

FILE * file_open_for_write(const char * filename, bool is_truncate)
{
    if (nullptr == filename || '\0' == filename[0])
    {
        return nullptr;
    }
#ifdef GOOFER_OS_IS_WIN
    const wchar_t * mode = (is_truncate ? L"wb" : L"ab");
    return _wfopen(utf8_to_unicode(filename).c_str(), mode);
#else
    const char * mode = (is_truncate ? "wb" : "ab");
    return fopen(utf8_to_ansi(filename).c_str(), mode);
#endif // GOOFER_OS_IS_WIN
}

void file_close(FILE *& file)
{
    if (nullptr != file)
    {
        fclose(file);
        file = nullptr;
    }
}

bool file_is_open(const FILE * file)
{
    return nullptr != file;
}

bool file_eof(FILE * file)
{
    return nullptr != file && 0 != feof(file);
}

bool file_write(FILE * file, const char * data, size_t data_len)
{
    if (nullptr == file)
    {
        return false;
    }
    while (data_len > 0)
    {
        size_t write_len = fwrite(data, 1, data_len, file);
        if (write_len < data_len)
        {
            data += write_len;
            data_len -= write_len;
        }
        else
        {
            break;
        }
    }
    return true;
}

bool file_read(FILE * file, char * buff, size_t buff_size, size_t & read_len)
{
    read_len = 0;
    if (nullptr == file)
    {
        return false;
    }
    while (!file_eof(file))
    {
        size_t data_len = fread(buff, 1, buff_size, file);
        read_len += data_len;
        if (data_len < buff_size)
        {
            buff += data_len;
            buff_size -= data_len;
        }
        else
        {
            break;
        }
    }
    return true;
}

bool file_seek(FILE * file, int64_t offset, int whence)
{
    if (nullptr == file)
    {
        return false;
    }
    fflush(file);
#ifdef GOOFER_OS_IS_WIN
    return 0 == static_cast<int64_t>(_fseeki64(file, static_cast<int64_t>(offset), whence));
#elif defined(GOOFER_OS_IS_MAC)
    return 0 == static_cast<int64_t>(fseeko(file, static_cast<off_t>(offset), whence));
#else
    return 0 == static_cast<int64_t>(fseeko64(file, static_cast<off64_t>(offset), whence));
#endif // GOOFER_OS_IS_WIN
}

bool file_seek(FILE * file, int64_t file_pos)
{
    return file_seek(file, file_pos, SEEK_SET);
}

bool file_tell(FILE * file, int64_t & file_pos)
{
    file_pos = -1;
    if (nullptr == file)
    {
        return false;
    }
    clearerr(file);
    fflush(file);
#ifdef GOOFER_OS_IS_WIN
    file_pos = static_cast<int64_t>(_ftelli64(file));
#elif defined(GOOFER_OS_IS_MAC)
    file_pos = static_cast<int64_t>(ftello(file));
#else
    file_pos = static_cast<int64_t>(ftello64(file));
#endif // GOOFER_OS_IS_WIN
    return file_pos >= 0;
}

bool file_flush(FILE * file)
{
    if (nullptr == file)
    {
        return false;
    }
    fflush(file);
    return true;
}

#ifdef GOOFER_OS_IS_WIN
static bool file_time_to_utc_time(const FILETIME & file_time, time_t & utc_time)
{
    SYSTEMTIME sys_time = { 0x0 };
    if (!FileTimeToSystemTime(&file_time, &sys_time))
    {
        return false;
    }

    struct tm tm_time = { 0x0 };
    tm_time.tm_isdst = 0;
    tm_time.tm_year = sys_time.wYear - 1900;
    tm_time.tm_mon = sys_time.wMonth - 1;
    tm_time.tm_mday = sys_time.wDay;
    tm_time.tm_hour = sys_time.wHour;
    tm_time.tm_sec = sys_time.wSecond;
    tm_time.tm_min = sys_time.wMinute;

    utc_time = mktime(&tm_time);
    if (static_cast<time_t>(-1) == utc_time)
    {
        return false;
    }

    utc_time -= goofer_get_timezone();

    return true;
}
#endif // GOOFER_OS_IS_WIN

bool goofer_access_safe(const char * path_name)
{
    if (nullptr == path_name)
    {
        return false;
    }

#ifdef GOOFER_OS_IS_WIN
    std::string pathname(path_name);
    if (!pathname.empty() && ':' == *pathname.rbegin())
    {
        pathname += g_directory_separator;
    }
    if (0 != _waccess(utf8_to_unicode(pathname).c_str(), GOOFER_F_OK))
    {
        return false;
    }
#else
    if (0 != access(utf8_to_ansi(path_name).c_str(), GOOFER_F_OK))
    {
        return false;
    }
#endif // GOOFER_OS_IS_WIN

    return true;
}

bool goofer_stat_safe(const char * path_name, goofer_stat_t & path_stat)
{
    memset(&path_stat, 0x0, sizeof(path_stat));

    if (!goofer_access_safe(path_name))
    {
        return false;
    }

    std::string pathname(path_name);

#ifdef GOOFER_OS_IS_WIN
    const std::string long_path_prefix("\\\\?\\");
    if (0 == strncmp(path_name, long_path_prefix.c_str(), long_path_prefix.size()))
    {
        WIN32_FILE_ATTRIBUTE_DATA path_attr = { 0x0 };
        if (GetFileAttributesExW(utf8_to_unicode(path_name).c_str(), GetFileExInfoStandard, &path_attr))
        {
            if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & path_attr.dwFileAttributes))
            {
                path_stat.st_mode = S_IFDIR;
                path_stat.st_size = 0;
            }
            else
            {
                path_stat.st_mode = S_IFREG;
                path_stat.st_size = ((static_cast<int64_t>(path_attr.nFileSizeHigh) << 32) | (static_cast<int64_t>(path_attr.nFileSizeLow)));
            }

            file_time_to_utc_time(path_attr.ftCreationTime, path_stat.st_ctime);
            file_time_to_utc_time(path_attr.ftLastWriteTime, path_stat.st_mtime);
            file_time_to_utc_time(path_attr.ftLastAccessTime, path_stat.st_atime);

            return true;
        }

        const std::string long_path_unc_prefix("\\\\?\\UNC\\");
        if (0 == strnicmp(path_name, long_path_unc_prefix.c_str(), long_path_unc_prefix.size()))
        {
            pathname = path_name + long_path_unc_prefix.size();
        }
        else
        {
            pathname = path_name + long_path_prefix.size();
        }

        if (pathname.size() < 2 || ':' != pathname[1])
        {
            pathname = "\\\\" + pathname;
        }
    }
#endif // GOOFER_OS_IS_WIN

    goofer_string_trim_tail(pathname, "\\/");

#ifdef GOOFER_OS_IS_WIN
    if (!pathname.empty() && ':' == *pathname.rbegin())
    {
        pathname += g_directory_separator;
    }
    if (0 != _wstat64(utf8_to_unicode(pathname).c_str(), &path_stat))
    {
        return false;
    }
#else
    if (pathname.empty())
    {
        pathname += g_directory_separator;
    }
    if (0 != goofer_stat(utf8_to_ansi(pathname).c_str(), &path_stat))
    {
        return false;
    }
#endif // GOOFER_OS_IS_WIN

    return true;
}

bool goofer_path_is_directory(const char * path_name, bool & path_is_directory)
{
    if (nullptr == path_name)
    {
        return false;
    }

    goofer_stat_t path_info = { 0x0 };
    if (!goofer_stat_safe(path_name, path_info))
    {
        return false;
    }

    path_is_directory = (S_IFDIR == (S_IFDIR & path_info.st_mode));

    return true;
}

bool goofer_mkdir_safe(const char * dir_name)
{
    if (nullptr == dir_name)
    {
        return false;
    }

#ifdef GOOFER_OS_IS_WIN
    return 0 == _wmkdir(utf8_to_unicode(dir_name).c_str());
#else
    return 0 == mkdir(utf8_to_ansi(dir_name).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif // GOOFER_OS_IS_WIN
}

bool goofer_rmdir_safe(const char * dir_name)
{
    if (nullptr == dir_name)
    {
        return false;
    }

#ifdef GOOFER_OS_IS_WIN
    return 0 == _wrmdir(utf8_to_unicode(dir_name).c_str());
#else
    return 0 == rmdir(utf8_to_ansi(dir_name).c_str());
#endif // GOOFER_OS_IS_WIN
}

bool goofer_rename_safe(const char * src_path_name, const char * dst_path_name)
{
#ifdef GOOFER_OS_IS_WIN
    return 0 == _wrename(utf8_to_unicode(src_path_name).c_str(), utf8_to_unicode(dst_path_name).c_str());
#else
    return 0 == rename(utf8_to_ansi(src_path_name).c_str(), utf8_to_ansi(dst_path_name).c_str());
#endif // GOOFER_OS_IS_WIN
}

bool goofer_unlink_safe(const char * file_name)
{
#ifdef GOOFER_OS_IS_WIN
    return 0 == _wunlink(utf8_to_unicode(file_name).c_str());
#else
    return 0 == unlink(utf8_to_ansi(file_name).c_str());
#endif // GOOFER_OS_IS_WIN
}

bool goofer_chmod_safe(const char * file_name)
{
#ifdef GOOFER_OS_IS_WIN
    return 0 == _wchmod(utf8_to_unicode(file_name).c_str(), S_IREAD | S_IWRITE);
#else
    return 0 == chmod(utf8_to_ansi(file_name).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif // GOOFER_OS_IS_WIN
}

bool goofer_truncate_safe(const char * file_name, std::size_t file_size)
{
#ifdef GOOFER_OS_IS_WIN
    HANDLE handle = CreateFileW(utf8_to_unicode(file_name).c_str(), GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (INVALID_HANDLE_VALUE == handle)
    {
        return false;
    }
    LARGE_INTEGER file_length;
    file_length.QuadPart = static_cast<LONGLONG>(file_size);
    bool ret = (SetFilePointerEx(handle, file_length, nullptr, FILE_BEGIN) && SetEndOfFile(handle));
    ::CloseHandle(handle);
    return ret;
#else
    return 0 == truncate(utf8_to_ansi(file_name).c_str(), static_cast<off_t>(file_size));
#endif // GOOFER_OS_IS_WIN
}

bool file_get_size(const char * filename, int64_t & filesize)
{
    goofer_stat_t fileinfo = { 0x0 };
    filesize = (goofer_stat_safe(filename, fileinfo) ? fileinfo.st_size : -1);
    return -1 != filesize;
}

bool file_get_time(const char * filename, int64_t & filetime)
{
    goofer_stat_t fileinfo = { 0x0 };
    filetime = (goofer_stat_safe(filename, fileinfo) ? fileinfo.st_mtime : -1);
    return -1 != filetime;
}

bool file_set_time(const char * filename, int64_t filetime)
{
#ifdef GOOFER_OS_IS_WIN
    struct _utimbuf timebuff = { static_cast<time_t>(time(nullptr)), static_cast<time_t>(filetime) };
    return 0 == _wutime(utf8_to_unicode(filename).c_str(), &timebuff);
#else
    struct utimbuf timebuff = { static_cast<time_t>(time(nullptr)), static_cast<time_t>(filetime) };
    return 0 == utime(utf8_to_ansi(filename).c_str(), &timebuff);
#endif // GOOFER_OS_IS_WIN
}

NAMESPACE_GOOFER_END
