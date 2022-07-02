#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "array.hpp"
#include "fwd.hpp"

// SparseArray structs
namespace kun
{
// data def
template<typename T, typename TS> union SparseArrayData
{
    T data;
    struct
    {
        TS prev;
        TS next;
    };
};
}// namespace kun

// SparseArray def
namespace kun
{
template<typename T, typename Alloc> class SparseArray
{
public:
    using SizeType = typename Alloc::SizeType;
    using DataType = SparseArrayData<T, SizeType>;

    // ctor & dtor

    // copy & move

    // assign & move assign

    // compare

    // getter

    // validate

    // memory op

    // add

    // emplace

    // append

    // remove

    // modify

    // find

    // contain

    // sort

private:
    // helper

private:
    u32*            m_bit_array;
    SizeType        m_bit_array_size;
    SizeType        m_first_free_index;
    SizeType        m_num_free_indices;
    Array<DataType> m_data;
};
}// namespace kun