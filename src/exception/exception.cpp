/********************************************************
 * Description : exception class
 * Date        : 2013-05-20 06:56:52
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include "exception/exception.h"

#ifdef _MSC_VER

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <dbghelp.h>
#include <psapi.h>
#include <winnt.h>
#include <cstring>
#include "log/log.h"
#include "time/time.h"
#include "string/string.h"
#include "filesystem/directory.h"

static char g_dump_save_root[512] = { 0x0 };
static char g_dump_save_file[512] = { 0x0 };
static UINT g_hit_unhandled_exception = 0;

LONG WINAPI unhandled_exception_handler(EXCEPTION_POINTERS * exception)
{
    if (0 != ::InterlockedCompareExchange(&g_hit_unhandled_exception, 1, 0))
    {
        return (EXCEPTION_CONTINUE_SEARCH);
    }

    char dump_file_name[1024] = { 0x0 };
    const std::string datetime(Goofer::goofer_get_datetime("", "", ""));
    Goofer::goofer_snprintf(dump_file_name, sizeof(dump_file_name), "%s\\%s-%s.dmp", g_dump_save_root, g_dump_save_file, datetime.c_str());

    HANDLE dump_handle = ::CreateFileA(dump_file_name, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (INVALID_HANDLE_VALUE != dump_handle)
    {
        MINIDUMP_EXCEPTION_INFORMATION exception_information = { ::GetCurrentThreadId(), exception, FALSE };
        DWORD flags = MiniDumpWithIndirectlyReferencedMemory | MiniDumpIgnoreInaccessibleMemory | MiniDumpWithUnloadedModules | MiniDumpWithThreadInfo;
        if (::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), dump_handle, static_cast<MINIDUMP_TYPE>(flags), &exception_information, nullptr, nullptr))
        {
            RUN_LOG_ERR("unhandled exception! minidump written to (%s)", dump_file_name);
        }
        else
        {
            RUN_LOG_ERR("unhandled exception! failed to write dump file (%s) with error (%d)", dump_file_name, ::GetLastError());
        }
        ::CloseHandle(dump_handle);
    }
    else
    {
        RUN_LOG_ERR("unhandled exception! failed to open dump file (%s) with error (%d)", dump_file_name, ::GetLastError());
    }

    return (EXCEPTION_EXECUTE_HANDLER);
}

#endif // _MSC_VER

NAMESPACE_GOOFER_BEGIN

Exception::Exception(const char * file, int line, const char * func, const char * msg)
    : m_message()
{
    if (nullptr == file)
    {
        file = "unknown_file";
    }

    if (0 > line)
    {
        line = -1;
    }

    const size_t INT_LENGH_BOUND = 16;
    char line_info[INT_LENGH_BOUND] = { 0 };
    goofer_snprintf(line_info, INT_LENGH_BOUND, "%d", line);

    if (nullptr == func)
    {
        func = "unknown_func";
    }

    if (nullptr == msg)
    {
        msg = "";
    }

    m_message += file;
    m_message += ":";
    m_message += line_info;
    m_message += " ";
    m_message += func;
    m_message += " throw: \"";
    m_message += msg;
    m_message += "\"";
}

const std::string & Exception::what() const
{
    return (m_message);
}

void goofer_set_dump_directory(const char * store_dir_root, const char * store_file_prefix)
{
#ifdef _MSC_VER
    if (nullptr == store_dir_root || nullptr == store_file_prefix || '\0' == store_dir_root[0] || '\0' == store_file_prefix[0])
    {
        std::string program_path;
        std::string program_dirname;
        std::string program_filename;
        if (goofer_get_current_process_pathname(program_path) && goofer_extract_path(program_path.c_str(), program_dirname, program_filename, true))
        {
            program_dirname += "dump";
            program_filename.substr(0, program_filename.rfind('.')).swap(program_filename);
            strncpy(g_dump_save_root, program_dirname.c_str(), sizeof(g_dump_save_root) - 1);
            strncpy(g_dump_save_file, program_filename.c_str(), sizeof(g_dump_save_file) - 1);
        }
        else
        {
            strncpy(g_dump_save_root, ".\\dump", sizeof(g_dump_save_root) - 1);
            strncpy(g_dump_save_file, "core", sizeof(g_dump_save_file) - 1);
        }
    }
    else
    {
        strncpy(g_dump_save_root, store_dir_root, sizeof(g_dump_save_root) - 1);
        strncpy(g_dump_save_file, store_file_prefix, sizeof(g_dump_save_file) - 1);
    }

    goofer_create_directory_recursive(g_dump_save_root);

    ::SetUnhandledExceptionFilter(unhandled_exception_handler);
#endif // _MSC_VER
}

NAMESPACE_GOOFER_END
