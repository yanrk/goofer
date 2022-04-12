/********************************************************
 * Description : thread local storage class in windows
 * Date        : 2015-02-27 16:01:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_WINDOWS_THREAD_LOCAL_STORAGE_H
#define GOOFER_WINDOWS_THREAD_LOCAL_STORAGE_H


#ifdef _MSC_VER


#include <windows.h>

#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API WindowsThreadLocalStorage : private Uncopy
{
public:
    WindowsThreadLocalStorage();
    ~WindowsThreadLocalStorage();

public:
    void set_value(void * thread_specific_value);
    void * get_value();

private:
    DWORD                   m_thread_key;
};

NAMESPACE_GOOFER_END


#endif // _MSC_VER


#endif // GOOFER_WINDOWS_THREAD_LOCAL_STORAGE_H
