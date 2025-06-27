/********************************************************
 * Description : thread local storage class in unix
 * Date        : 2015-02-27 15:23:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_UNIX_THREAD_LOCAL_STORAGE_H
#define GOOFER_UNIX_THREAD_LOCAL_STORAGE_H


#include "common/common.h"

#ifndef GOOFER_OS_IS_WIN


#include <pthread.h>

#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API UnixThreadLocalStorage : private Uncopy
{
public:
    UnixThreadLocalStorage();
    ~UnixThreadLocalStorage();

public:
    void set_value(void * thread_specific_value);
    void * get_value();

private:
    pthread_key_t               m_thread_key;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_OS_IS_WIN


#endif // GOOFER_UNIX_THREAD_LOCAL_STORAGE_H
