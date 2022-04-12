/********************************************************
 * Description : make sure program only runs an entity
 * Date        : 2015-07-02 10:55:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_EXCLUSIVE_H
#define GOOFER_EXCLUSIVE_H


#include "common/common.h"
#include "utility/uncopy.h"
#include "utility/singleton.h"

NAMESPACE_GOOFER_BEGIN

class GOOFER_API MutexApplication : private Uncopy
{
public:
    bool exclusive(const char * exclusive_unique_name, bool global_exclusive = true);

private:
    MutexApplication();
    ~MutexApplication();

private:
    friend class Singleton<MutexApplication>;

private:
    bool                        m_exclusive;
    size_t                      m_unique_id;
};

GOOFER_CXX_API(bool) mutex_application_is_exist(const char * exclusive_unique_name, bool global_exclusive = true);

NAMESPACE_GOOFER_END


#endif // GOOFER_EXCLUSIVE_H
