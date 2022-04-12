/********************************************************
 * Description : macros of system standard I/O
 * Date        : 2013-05-23 11:46:45
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_SYSTEM_IO_H
#define GOOFER_SYSTEM_IO_H


#ifdef _MSC_VER
    #include <io.h>
    #include <direct.h>
#else
    #include <unistd.h>
#endif // _MSC_VER

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _MSC_VER
    #define GOOFER_R_OK     4
    #define GOOFER_W_OK     2
    #define GOOFER_X_OK     1
    #define GOOFER_F_OK     0
#else
    #define GOOFER_R_OK     R_OK
    #define GOOFER_W_OK     W_OK
    #define GOOFER_X_OK     X_OK
    #define GOOFER_F_OK     F_OK
#endif // _MSC_VER

#ifdef _MSC_VER
    #define goofer_umask()            (0)
    #define goofer_chmod(file)        ::chmod(file, S_IREAD | S_IWRITE)
    #define goofer_open(file, mode)   ::_open(file, mode, S_IREAD | S_IWRITE)
    #define goofer_close              ::_close
    #define goofer_lseek              ::_lseek
    #define goofer_read               ::_read
    #define goofer_write              ::_write
    #define goofer_stat               ::_stat64
    #define goofer_stat_t             struct _stat64
    #define goofer_access             ::_access
    #define goofer_mkdir(dir)         ::_mkdir(dir)
    #define goofer_rmdir(dir)         ::_rmdir(dir)
    #define goofer_getcwd(dir, len)   ::_getcwd(dir, len)
    #define goofer_setcwd(dir)        ::_chdir(dir)
    #define goofer_rename             ::rename
    #define goofer_unlink             ::_unlink
    #define goofer_sync               ::_commit
#else
    #define goofer_umask()            ::umask(S_IWGRP | S_IWOTH)
    #define goofer_chmod(file)        ::chmod(file, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
    #define goofer_open(file, mode)   ::open(file, mode, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
    #define goofer_close              ::close
    #define goofer_lseek              ::lseek
    #define goofer_read               ::read
    #define goofer_write              ::write
#ifdef _MAC_OS
    #define goofer_stat               ::stat
    #define goofer_stat_t             struct stat
#else
    #define goofer_stat               ::stat64
    #define goofer_stat_t             struct stat64
#endif // _MAC_OS
    #define goofer_access             ::access
    #define goofer_mkdir(dir)         ::mkdir(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
    #define goofer_rmdir(dir)         ::rmdir(dir)
    #define goofer_getcwd(dir, len)   ::getcwd(dir, len)
    #define goofer_setcwd(dir)        ::chdir(dir)
    #define goofer_rename             ::rename
    #define goofer_unlink             ::unlink
    #define goofer_sync               ::fsync
#endif // _MSC_VER


#endif // GOOFER_SYSTEM_IO_H
