#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/algo/functor.hpp"
#include "sparse_array.hpp"
#include "fwd.hpp"

// USet config
namespace kun
{
template<typename T, bool MultiKey> struct USetConfigDefault
{
    using KeyType = T;
    using KeyMapperType = MapFwd<T>;
    using HashType = Size;
    using HasherType = Hash<KeyType>;
    using ComparerType = Equal<KeyType>;

    static constexpr bool multi_key = MultiKey;
};
}// namespace kun

// USet data
namespace kun
{
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

// USet def
namespace kun
{
template<typename T, typename Alloc, typename Config> class USet
{
public:
    using SizeType = typename Alloc::SizeType;
    using HashType = typename Config::HashType;
    using KeyType = typename Config::KeyType;
    using keyMapperType = typename Config::KeyMapperType;
    using HasherType = typename Config::HasherType;
    using ComparerType = typename Config::ComparerType;
    using DataType = USetData<T, SizeType, HashType>;
    using DataInfo = USetDataInfo<T, SizeType>;
    using CDataInfo = USetDataInfo<const T, SizeType>;
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
    bool           needRehash();
    void           rehash();
    bool           rehashIfNeed();
    KeyType&       keyOf(T& v) const;
    const KeyType& keyOf(const T& v) const;
    bool           keyEqual(const T& a, const T& b) const;
    HashType       hashOf(const T& v) const;

    // add (add or assign)
    DataInfo add(const T& v);
    DataInfo add(T&& v);
    DataInfo addHashed(const T& v, HashType hash);
    DataInfo addHashed(T&& v, HashType hash);

    // add anyway (add but never check existence)
    DataInfo addAnyway(const T& v);
    DataInfo addAnyway(T&& v);
    DataInfo addAnywayHashed(const T& v, HashType hash);
    DataInfo addAnywayHashed(T&& v, HashType hash);

    // try add (first check existence, then add, never assign)
    DataInfo tryAdd(const T& v);
    DataInfo tryAdd(T&& v);
    DataInfo tryAddHashed(const T& v, HashType hash);
    DataInfo tryAddHashed(T&& v, HashType hash);
    template<typename AsType, typename AsHasher = Hash<AsType>, typename AsComparer = Equal<>>
    DataInfo tryAddAs(AsType&& v, AsHasher&& hasher = AsHasher(), AsComparer&& comparer = AsComparer());

    // emplace
    template<typename... Args> DataInfo emplace(Args&&... args);
    template<typename... Args> DataInfo emplaceHashed(HashType hash, Args&&... args);
    template<typename... Args> DataInfo emplaceAnyway(Args&&... args);
    template<typename... Args> DataInfo emplaceAnywayHashed(HashType hash, Args&&... args);

    // append
    void append(const USet& set);
    void append(std::initializer_list<T> init_list);
    void append(T* p, SizeType n);

    // remove
    SizeType remove(const KeyType& key);
    SizeType removeHashed(const KeyType& key, HashType hash);
    SizeType remvoeAll(const KeyType& key);                     // [multi set extend]
    SizeType removeAllHashed(const KeyType& key, HashType hash);// [multi set extend]

    // remove as
    template<typename AsType, typename AsHasher = Hash<AsType>, typename AsComparer = Equal<>>
    SizeType removeAs(AsType&& v, AsHasher&& hasher = AsHasher(), AsComparer&& comparer = AsComparer());
    template<typename AsType, typename AsHasher = Hash<AsType>, typename AsComparer = Equal<>>
    SizeType removeAllAs(AsType&& v, AsHasher&& hasher = AsHasher(), AsComparer&& comparer = AsComparer());// [multi set extend]

    // modify
    T&       operator[](SizeType index);
    const T& operator[](SizeType index) const;

    // find
    DataInfo  find(const KeyType& key);
    CDataInfo find(const KeyType& key) const;
    DataInfo  findHashed(const KeyType& key, HashType hash);
    CDataInfo findHashed(const KeyType& key, HashType hash) const;

    // find as
    template<typename AsType, typename AsHasher = Hash<AsType>, typename AsComparer = Equal<>>
    DataInfo findAs(AsType&& v, AsHasher&& hasher = AsHasher(), AsComparer&& comparer = AsComparer());
    template<typename AsType, typename AsHasher = Hash<AsType>, typename AsComparer = Equal<>>
    CDataInfo findAs(AsType&& v, AsHasher&& hasher = AsHasher(), AsComparer&& comparer = AsComparer()) const;

    // contain
    bool     contain(const KeyType& key) const;
    bool     containHashed(const KeyType& key, HashType hash) const;
    SizeType count(const KeyType& key) const;                     // [multi set extend]
    SizeType countHashed(const KeyType& key, HashType hash) const;// [multi set extend]

    // contain as
    template<typename AsType, typename AsHasher = Hash<AsType>, typename AsComparer = Equal<>>
    bool containAs(AsType&& v, AsHasher&& hasher = AsHasher(), AsComparer&& comparer = AsComparer()) const;
    template<typename AsType, typename AsHasher = Hash<AsType>, typename AsComparer = Equal<>>
    SizeType countAs(AsType&& v, AsHasher&& hasher = AsHasher(), AsComparer&& comparer = AsComparer()) const;// [multi set extend]

    // sort
    template<typename TP = Less<T>> void sort(TP&& p = TP());
    template<typename TP = Less<T>> void sortStable(TP&& p = TP());

    // set ops
    USet operator&(const USet& rhs) const; // intersect
    USet operator|(const USet& rhs) const; // union
    USet operator^(const USet& rhs) const; // difference
    USet operator-(const USet& rhs) const; // sub
    bool isSubsetOf(const USet& rhs) const;// sub set

    // support foreach

private:
    // helpers
    SizeType  _calcBucketSize(SizeType data_size) const;
    void      _cleanBucket() const;
    SizeType  _bucketIndex(SizeType hash) const;
    SizeType& _bucketData(SizeType hash) const;
    DataInfo  _linkToBucketOrAssign(SizeType index, DataType& data);

private:
    mutable SizeType* m_bucket;
    mutable SizeType  m_bucket_size;
    DataArr           m_data;
};
}// namespace kun