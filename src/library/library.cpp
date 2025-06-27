/********************************************************
 * Description : library dynamic import functions
 * Date        : 2015-03-24 09:22:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include "library/library.h"

#ifdef GOOFER_OS_IS_WIN
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif // GOOFER_OS_IS_WIN

#include "string/string.h"

NAMESPACE_GOOFER_BEGIN

void * goofer_library_acquire(const char * filename)
{
    if (nullptr == filename)
    {
        return (nullptr);
    }

#ifdef GOOFER_OS_IS_WIN
    return (reinterpret_cast<void *>(LoadLibraryA(filename)));
#else
    return (dlopen(filename, RTLD_LAZY | RTLD_LOCAL));
#endif // GOOFER_OS_IS_WIN
}

int goofer_library_release(void * library)
{
    if (nullptr == library)
    {
        return (0);
    }

#ifdef GOOFER_OS_IS_WIN
    return (FreeLibrary(reinterpret_cast<HMODULE>(library)) ? 1 : 0);
#else
    return (0 == dlclose(library) ? 1 : 0);
#endif // GOOFER_OS_IS_WIN
}

void * goofer_library_symbol(void * library, const char * symbol)
{
    if (nullptr == library)
    {
        return (nullptr);
    }

    if (nullptr == symbol)
    {
        return (nullptr);
    }

#ifdef GOOFER_OS_IS_WIN
    return (GetProcAddress(reinterpret_cast<HMODULE>(library), symbol));
#else
    return (dlsym(library, symbol));
#endif // GOOFER_OS_IS_WIN
}

const char * goofer_library_error(void)
{
    static char library_error[128] = { 0 };
#ifdef GOOFER_OS_IS_WIN
    DWORD error_code = GetLastError();
    if (0 == error_code)
    {
        return ("");
    }
    goofer_snprintf(library_error, sizeof(library_error), "library error: %d", error_code);
#else
    const char * error_info = dlerror();
    if (nullptr == error_info)
    {
        return ("");
    }
    goofer_snprintf(library_error, sizeof(library_error), "library error: %s", error_info);
#endif // GOOFER_OS_IS_WIN
    return (library_error);
}

Library::Library()
    : m_name()
    , m_what()
    , m_library(nullptr)
{

}

Library::~Library()
{
    release();
}

bool Library::acquire(const char * library_name)
{
    release();

    if (nullptr == library_name)
    {
        m_what = "library name is nullptr";
        return (false);
    }

    m_name = library_name;
    m_library = goofer_library_acquire(library_name);
    if (nullptr == m_library)
    {
        m_what = goofer_library_error();
        return (false);
    }

    return (true);
}

void Library::release()
{
    m_name.clear();
    m_what.clear();
    if (nullptr != m_library)
    {
        if (0 == goofer_library_release(m_library))
        {
            m_what = goofer_library_error();
        }
        m_library = 0;
    }
}

const std::string & Library::name() const
{
    return (m_name);
}

const std::string & Library::what() const
{
    return (m_what);
}

NAMESPACE_GOOFER_END
