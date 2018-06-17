/********************************************************
 * Description : macros of goofer
 * Data        : 2013-05-18 17:10:05
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_MACROS_H
#define GOOFER_MACROS_H


#ifdef _MSC_VER
    #define GOOFER_CDECL           __cdecl
    #define GOOFER_STDCALL         __stdcall
    #ifdef EXPORT_GOOFER_DLL
        #define GOOFER_API         __declspec(dllexport)
    #else
        #ifdef USE_GOOFER_DLL
            #define GOOFER_API     __declspec(dllimport)
        #else
            #define GOOFER_API
        #endif // USE_GOOFER_DLL
    #endif // EXPORT_GOOFER_DLL
#else
    #define GOOFER_CDECL
    #define GOOFER_STDCALL
    #define GOOFER_API
#endif // _MSC_VER

#ifdef __cplusplus
    #define GOOFER_C_API(return_type)            extern "C" GOOFER_API return_type GOOFER_CDECL
    #define GOOFER_EXTERN_TYPE(variable_type)    extern "C" GOOFER_API variable_type
#else
    #define GOOFER_C_API(return_type)            extern     GOOFER_API return_type GOOFER_CDECL
    #define GOOFER_EXTERN_TYPE(variable_type)    extern     GOOFER_API variable_type
#endif // __cplusplus

#define GOOFER_CXX_API(return_type)              extern     GOOFER_API return_type GOOFER_CDECL

#ifdef __cplusplus
    #define GOOFER_INLINE             inline
    #define GOOFER_DEFAULT_ARG(var)   = var
#else
    #define GOOFER_INLINE             static
    #define GOOFER_DEFAULT_ARG(var)
#endif // __cplusplus

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    #define GOOFER_I64_VAL(n)   n##I64
    #define GOOFER_U64_VAL(n)   n##UI64
    #define GOOFER_U64_FMT      "%I64u"
    #define GOOFER_I64_FMT      "%I64d"
    #define GOOFER_I64_ARG(x)   ((__int64)(x))
    #define GOOFER_U64_ARG(x)   ((unsigned __int64)(x))
#else
    #define GOOFER_I64_VAL(n)   n##LL
    #define GOOFER_U64_VAL(n)   n##ULL
    #define GOOFER_U64_FMT      "%llu"
    #define GOOFER_I64_FMT      "%lld"
    #define GOOFER_I64_ARG(x)   ((long long)(x))
    #define GOOFER_U64_ARG(x)   ((unsigned long long)(x))
#endif // defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)

#if (defined(__APPLE__) && defined(__GNUC__)) || defined(__MACOSX__) || defined(macintosh)
    #define _MAC_OS
#else
    #undef  _MAC_OS
#endif // (defined(__APPLE__) && defined(__GNUC__)) || defined(__MACOSX__) || defined(macintosh)

#ifdef _MSC_VER
    #define GOOFER_LIBRARY(path, library)   path "/" library ".dll"
#elif defined(_MAC_OS)
    #define GOOFER_LIBRARY(path, library)   path "/lib" library ".dylib"
#else
    #define GOOFER_LIBRARY(path, library)   path "/lib" library ".so"
#endif // _MSC_VER

#ifdef _MSC_VER
    #define goofer_system_error()                (::GetLastError())
#else
    #define goofer_system_error()                (errno + 0)
#endif // _MSC_VER


#endif // GOOFER_MACROS_H
