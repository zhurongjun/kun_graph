#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "sparse_array.hpp"
#include "fwd.hpp"

// USet config
namespace kun
{
template<typename T, bool MultiKey> struct USetConfigDefault
{
    using KeyType = T;
    using HashType = Size;

    static constexpr bool multi_key = MultiKey;

    KUN_INLINE static const KeyType& key(const T& v) { return v; }
    KUN_INLINE static bool           equal(const KeyType& a, const KeyType& b) { return a == b; }
    KUN_INLINE static HashType       hash(const KeyType& k) { return Hash<KeyType>()(k); }
};
}// namespace kun

// USet element
namespace kun
{
template<typename T, typename TS, typename HashType> struct USetData
{
    T          data;
    HashType   hash;
    mutable TS next;
};
}// namespace kun

// USet def
namespace kun
{
template<typename T, typename Alloc, typename Config> class USet
{
public:
    using SizeType = typename Alloc::SizeType;
    using HashType = typename Config::HashType;
    using KeyType = typename Config::KeyType;
    using DataType = USetData<T, SizeType, HashType>;

public:
    // ctor & dtor

    // copy & move

    // assign & move assign

    // compare

    // getter

    // validate

    // memory op

    // add

    // add as

    // emplace

    // append

    // remove

    // remove as

    // use as sparse array? remove all & remove by

    // modify

    // find

    // find as

    // contain

    // sort

    // static? set ops

private:
    mutable SizeType*            m_bucket;
    mutable SizeType             m_bucket_size;
    SparseArray<DataType, Alloc> m_data;
};
}// namespace kun