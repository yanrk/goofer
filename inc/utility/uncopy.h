/********************************************************
 * Description : base class which not support copy
 * Data        : 2013-05-20 12:24:45
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_UNCOPY_H
#define GOOFER_UNCOPY_H


#include "common/common.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API Uncopy
{
public:
    Uncopy();
    ~Uncopy();

private:
    Uncopy(const Uncopy &);
    Uncopy & operator = (const Uncopy &);
};

NAMESPACE_GOOFER_END


#endif // GOOFER_UNCOPY_H
