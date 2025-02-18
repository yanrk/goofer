/********************************************************
 * Description : singleton template of boost
 * Date        : 2013-10-19 09:14:45
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_SINGLETON_H
#define GOOFER_SINGLETON_H


#include "common/common.h"

NAMESPACE_GOOFER_BEGIN

template <typename T>
class Singleton
{
public:
    static T & instance()
    {
        static T obj;
        creator.do_nothing();
        return (obj);
    }

private:
    struct object_creator
    {
        object_creator()
        {
            Singleton<T>::instance();
        }

        inline void do_nothing() const { }
    };

    static object_creator creator;
};

template <typename T> typename Singleton<T>::object_creator Singleton<T>::creator;

NAMESPACE_GOOFER_END


#endif // GOOFER_SINGLETON_H
