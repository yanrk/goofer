/********************************************************
 * Description : resource guard template
 * Data        : 2013-05-20 12:48:51
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_GUARD_H
#define GOOFER_GUARD_H


#include <cassert>
#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

template <typename ResourceType>
class Guard : private Uncopy
{
public:
    Guard(ResourceType & resource);
    ~Guard();

public:
    void acquire();
    void release();

private:
    bool             m_owned;
    ResourceType   & m_resource;
};

template <typename ResourceType>
Guard<ResourceType>::Guard(ResourceType & resource)
    : m_owned(false), m_resource(resource)
{
    acquire();
}

template <typename ResourceType>
Guard<ResourceType>::~Guard()
{
    release();
}

template <typename ResourceType>
void Guard<ResourceType>::acquire()
{
    if (!m_owned)
    {
        m_owned = m_resource.acquire();
        assert(m_owned);
    }
}

template <typename ResourceType>
void Guard<ResourceType>::release()
{
    if (m_owned)
    {
        m_resource.release();
        m_owned = false;
    }
}

NAMESPACE_GOOFER_END


#endif // GOOFER_GUARD_H
