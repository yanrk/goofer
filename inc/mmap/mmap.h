/********************************************************
 * Description : virtual memory map class
 * Date        : 2022-01-24 16:59:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2022
 ********************************************************/

#ifndef GOOFER_MMAP_H
#define GOOFER_MMAP_H


#include "common/common.h"

#ifdef GOOFER_OS_IS_WIN
    #include "mmap/windows_mmap.h"
#else
    #include "mmap/unix_mmap.h"
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_BEGIN

#ifdef GOOFER_OS_IS_WIN
    typedef WindowsMemMap               MemMap;
    typedef WindowsFileMap              FileMap;
#else
    typedef UnixMemMap                  MemMap;
    typedef UnixFileMap                 FileMap;
#endif // GOOFER_OS_IS_WIN

NAMESPACE_GOOFER_END


#endif // GOOFER_MMAP_H
