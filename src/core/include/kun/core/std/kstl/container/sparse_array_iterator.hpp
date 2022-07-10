#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "bit_iterator.hpp"

// SparseArray structs
namespace kun
{
// data def
template<typename T, typename TS> union SparseArrayData
{
    // free linked list
    struct
    {
        TS prev;
        TS next;
    };

    // data
    T data;
};
// data info
template<typename T, typename TS> struct SparseArrayDataInfo
{
    T* data;
    TS index;

    KUN_INLINE SparseArrayDataInfo()
        : data(nullptr)
        , index((TS)npos)
    {
    }
    KUN_INLINE SparseArrayDataInfo(T* data, TS index)
        : data(data)
        , index(index)
    {
    }
    KUN_INLINE    operator bool() const { return data != nullptr; }
    KUN_INLINE T& operator*() const { return *data; }
    KUN_INLINE T* operator->() const { return data; }
};
}// namespace kun

// SparseArray iterator
namespace kun
{
template<typename T, typename TS, bool Const> class SparseArrayIt
{
public:
    using DataType = std::conditional_t<Const, const SparseArrayData<T, TS>, SparseArrayData<T, TS>>;
    using ValueType = std::conditional_t<Const, const T, T>;
    using BitItType = TrueBitIt<TS>;

    KUN_INLINE explicit SparseArrayIt(DataType* array, TS array_size, const u32* bit_array, TS start = 0)
        : m_array(array)
        , m_bit_it(bit_array, array_size, start)
    {
    }

    KUN_INLINE SparseArrayIt& operator++()
    {
        ++m_bit_it;
        return *this;
    }
    KUN_INLINE TS         index() const { return m_bit_it.index(); }
    KUN_INLINE bool       operator==(const SparseArrayIt& rhs) const { return m_bit_it == rhs.m_bit_it && m_array == rhs.m_array; }
    KUN_INLINE bool       operator!=(const SparseArrayIt& rhs) const { return !(*this == rhs); }
    KUN_INLINE            operator bool() const { return (bool)m_bit_it; }
    KUN_INLINE bool       operator!() const { return !(bool)*this; }
    KUN_INLINE ValueType& operator*() const { return m_array[index()].data; }
    KUN_INLINE ValueType* operator->() const { return &m_array[index()].data; }

private:
    DataType* m_array;
    BitItType m_bit_it;
};
}// namespace kun