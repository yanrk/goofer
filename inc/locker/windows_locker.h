/********************************************************
 * Description : locker classes in windows
 * Data        : 2013-05-20 09:02:33
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_WINDOWS_LOCKER_H
#define GOOFER_WINDOWS_LOCKER_H


#ifdef _MSC_VER


#include <windows.h>

#include <string>
#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API WindowsThreadLocker : private Uncopy
{
public:
    WindowsThreadLocker(const char * name = nullptr);
    ~WindowsThreadLocker();

public:
    bool acquire() const;
    void release() const;
    bool try_acquire() const; /* not support, alway be false */

private:
    std::string                m_name;
    mutable CRITICAL_SECTION   m_locker;
};


class GOOFER_API WindowsProcessLocker : private Uncopy
{
public:
    WindowsProcessLocker(const char * name = nullptr);
    ~WindowsProcessLocker();

public:
    bool acquire() const;
    void release() const;
    bool try_acquire() const;

private:
    std::string      m_name;
    mutable HANDLE   m_locker;
};

NAMESPACE_GOOFER_END


#endif // _MSC_VER


#endif // GOOFER_WINDOWS_LOCKER_H
