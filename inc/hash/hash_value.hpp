/********************************************************
 * Description : hash value function template
 * Data        : 2018-08-24 10:12:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_HASH_VALUE_HPP
#define GOOFER_HASH_VALUE_HPP


#include <functional>
#include "common/common.h"

NAMESPACE_GOOFER_BEGIN

template <typename T>
inline void hash_combine(std::size_t & seed, const T & data)
{
    std::size_t value = std::hash<T>()(data);
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename Iter>
inline std::size_t hash_range(Iter first, Iter last)
{
    std::size_t seed = 0;
    while (first != last)
    {
        hash_combine(seed, *first);
        ++first;
    }
    return (seed);
}

template <typename T>
inline std::size_t hash_value(const T & t)
{
    return (std::hash<T>()(t));
}

template <typename T, unsigned int N>
inline std::size_t hash_value(const T (&t) [N])
{
    return (hash_range(t, t + N));
}

template <typename T, unsigned int N>
inline std::size_t hash_value(T (&t) [N])
{
    return (hash_range(t, t + N));
}

NAMESPACE_GOOFER_END


#endif // GOOFER_HASH_VALUE_HPP
