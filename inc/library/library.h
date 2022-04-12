/********************************************************
 * Description : library dynamic import functions & class
 * Date        : 2015-03-24 09:22:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_LIBRARY_H
#define GOOFER_LIBRARY_H


#include <string>
#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

GOOFER_C_API(void *) goofer_library_acquire(const char * filename); /* null: failure */
GOOFER_C_API(int) goofer_library_release(void * library); /* 1: success, 0: failure */
GOOFER_C_API(void *) goofer_library_symbol(void * library, const char * symbol); /* null: failure */
GOOFER_C_API(const char *) goofer_library_error(void); /* "": success */

class GOOFER_API Library : private Uncopy
{
public:
    Library();
    ~Library();

public:
    bool acquire(const char * library_name);
    void release();

public:
    const std::string & name() const;
    const std::string & what() const;

public:
    template <typename FuncPtr> FuncPtr get(const char * function_name);

private:
    std::string          m_name;
    std::string          m_what;
    void               * m_library;
};

template <typename FuncPtr>
FuncPtr Library::get(const char * function_name)
{
    if (nullptr == m_library)
    {
        m_what = "library is uninitialized";
        return (nullptr);
    }

    if (nullptr == function_name)
    {
        m_what = "function name is nullptr";
        return (nullptr);
    }

    void * func_ptr = goofer_library_symbol(m_library, function_name);
    if (nullptr == func_ptr)
    {
        m_what = goofer_library_error();
        return (nullptr);
    }

    m_what.clear();
    return (reinterpret_cast<FuncPtr>(func_ptr));
}

NAMESPACE_GOOFER_END


#endif // GOOFER_LIBRARY_H
