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
    using DataArr = SparseArray<DataType, Alloc>;

public:
    // ctor & dtor
    USet(Alloc alloc = Alloc());
    USet(SizeType reserve_size, Alloc alloc = Alloc());
    USet(const T* p, SizeType n, Alloc alloc = Alloc());
    USet(std::initializer_list<T> init_list, Alloc alloc = Alloc());
    USet(const Array<T>& arr, Alloc alloc = Alloc());
    ~USet();

    // copy & move
    USet(const USet& other);
    USet(USet&& other);

    // assign & move assign
    USet& operator=(const USet& rhs);
    USet& operator=(USet&& rhs);

    // compare
    bool operator==(const USet& rhs);
    bool operator!=(const USet& rhs);

    // getter
    bool            isCompact() const;
    SizeType        size() const;
    SizeType        sparseSize() const;
    SizeType        holeSize() const;
    SizeType        capacity() const;
    SizeType        slack() const;
    SizeType        bitArraySize() const;
    SizeType        bucketSize() const;
    bool            empty() const;
    DataArr&        data();
    const DataArr&  data() const;
    SizeType*       bucket();
    const SizeType* bucket() const;
    Alloc&          allocator();
    const Alloc&    allocator() const;

    // validate
    bool hasData(SizeType idx) const;
    bool isHole(SizeType idx) const;
    bool isValidIndex(SizeType idx) const;
    bool isValidPointer(const T* p) const;

    // memory op
    void clear();
    void release(SizeType capacity = 0);
    void reserve(SizeType capacity);
    void shrink();
    void compact();
    void compactStable();

    // hash op
    bool needRehash();
    void rehash();
    void rehashIfNeed();

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
    mutable SizeType* m_bucket;
    mutable SizeType  m_bucket_size;
    DataArr           m_data;
};
}// namespace kun