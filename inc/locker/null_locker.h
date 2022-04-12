/********************************************************
 * Description : null locker class
 * Date        : 2013-05-30 16:45:57
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_NULL_LOCKER_H
#define GOOFER_NULL_LOCKER_H


#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API NullLocker : private Uncopy
{
public:
    NullLocker(const char * name = nullptr);
    ~NullLocker();

public:
    bool acquire() const;
    void release() const;
    bool try_acquire() const;
};

NAMESPACE_GOOFER_END


#endif // GOOFER_NULL_LOCKER_H
