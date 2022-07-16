#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "sparse_array_iterator.hpp"

// USet structs
namespace kun
{
// data def
template<typename T, typename TS, typename HashType> struct USetData
{
    T          data;
    HashType   hash;
    mutable TS next;
};
// data info
template<typename T, typename TS> struct USetDataInfo
{
    T*   data;
    TS   index;
    bool already_exist;// used by add or emplace, always be false in other case

    KUN_INLINE USetDataInfo()
        : data(nullptr)
        , index((TS)npos)
        , already_exist(false)
    {
    }
    KUN_INLINE USetDataInfo(T* data, TS index, bool already_exist = false)
        : data(data)
        , index(index)
        , already_exist(already_exist)
    {
    }
    KUN_INLINE    operator bool() { return data != nullptr; }
    KUN_INLINE T& operator*() const { return *data; }
    KUN_INLINE T* operator->() const { return data; }
};
}// namespace kun

// USet iterator
namespace kun
{
template<typename T, typename TS, typename TH, bool Const> class USetIt
{
    using DataType = USetData<T, TS, TH>;
    using SparseDataType = std::conditional_t<Const, const SparseArrayData<DataType, TS>, SparseArrayData<DataType, TS>>;
    using ValueType = std::conditional_t<Const, const T, T>;
    using BitItType = TrueBitIt<TS>;

    KUN_INLINE explicit USetIt(SparseDataType* array, TS array_size, const u32* bit_array, TS start = 0)
        : m_array(array)
        , m_bit_it(bit_array, array_size, start)
    {
    }

    // impl cpp iterator
    KUN_INLINE USetIt& operator++()
    {
        ++m_bit_it;
        return *this;
    }
    KUN_INLINE bool       operator==(const USetIt& rhs) const { return m_bit_it == rhs.m_bit_it && m_array == rhs.m_array; }
    KUN_INLINE bool       operator!=(const USetIt& rhs) const { return !(*this == rhs); }
    KUN_INLINE            operator bool() const { return (bool)m_bit_it; }
    KUN_INLINE bool       operator!() const { return !(bool)*this; }
    KUN_INLINE ValueType& operator*() const { return m_array[index()].data.data; }
    KUN_INLINE ValueType* operator->() const { return &m_array[index()].data.data; }

    // other data
    KUN_INLINE TS index() const { return m_bit_it.index(); }
    KUN_INLINE TH hash() const { return m_array[index()].data.hash; }

private:
    SparseDataType* m_array;
    BitItType       m_bit_it;
};
}// namespace kun
