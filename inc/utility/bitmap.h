/********************************************************
 * Description : bit-map
 * Date        : 2014-12-26 18:06:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#ifndef GOOFER_BITMAP_H
#define GOOFER_BITMAP_H


#include <cstdint>
#include <cstring>
#include "common/common.h"
#include "utility/uncopy.h"

NAMESPACE_GOOFER_BEGIN

template <typename T>
class BitMap : private Uncopy
{
public:
    BitMap(T min, T max);
    ~BitMap();

public:
    bool reset();
    bool set(T val);
    bool clear(T val);
    bool test(T val) const;

private:
    const T   m_min;
    const T   m_max;
    size_t    m_siz;
    uint8_t * m_bit;
};

template <typename T>
BitMap<T>::BitMap(T min, T max)
    : m_min(min), m_max(max), m_siz((max - min + 1 + 7) >> 3), m_bit(nullptr)
{
    GOOFER_NEW_ARR(m_bit, uint8_t, m_siz);
    reset();
}

template <typename T>
BitMap<T>::~BitMap()
{
    GOOFER_DEL_ARR(m_bit);
}

template <typename T>
bool BitMap<T>::reset()
{
    if (nullptr == m_bit)
    {
        return (false);
    }
    memset(m_bit, 0x0, m_siz);
    return (true);
}

template <typename T>
bool BitMap<T>::set(T val)
{
    if (nullptr == m_bit || val < m_min || val > m_max)
    {
        return (false);
    }
    val -= m_min;
    m_bit[val >> 3] |= (0x01 << (val & 0x07));
    return (true);
}

template <typename T>
bool BitMap<T>::clear(T val)
{
    if (nullptr == m_bit || val < m_min || val > m_max)
    {
        return (false);
    }
    val -= m_min;
    m_bit[val >> 3] &= ~(0x01 << (val & 0x07));
    return (true);
}

template <typename T>
bool BitMap<T>::test(T val) const
{
    if (nullptr == m_bit || val < m_min || val > m_max)
    {
        return (false);
    }
    val -= m_min;
    return (0 != (m_bit[val >> 3] & (0x01 << (val & 0x07))));
}

NAMESPACE_GOOFER_END


#endif // GOOFER_BITMAP_H
