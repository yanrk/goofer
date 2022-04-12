/********************************************************
 * Description : macros of namespace
 * Date        : 2013-05-18 17:51:57
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_NAMESPACE_H
#define GOOFER_NAMESPACE_H


#ifdef __cplusplus
    namespace Goofer
    {

    }

    #define NAMESPACE_GOOFER_BEGIN  namespace Goofer {
    #define NAMESPACE_GOOFER_END    }
    #define USING_NAMESPACE_GOOFER  using namespace Goofer;
#else
    #define NAMESPACE_GOOFER_BEGIN
    #define NAMESPACE_GOOFER_END
    #define USING_NAMESPACE_GOOFER
#endif // __cplusplus


#endif // GOOFER_NAMESPACE_H
