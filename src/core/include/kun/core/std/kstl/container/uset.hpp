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
template<typename T, typename Config, typename Alloc> class USet
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

    // ctor & dtor
    USet(Alloc alloc = Alloc());
    USet(SizeType reserve_size, Alloc alloc = Alloc());
    USet(const T* p, SizeType n, Alloc alloc = Alloc());
    USet(std::initializer_list<T> init_list, Alloc alloc = Alloc());
    ~USet();

    // copy & move
    USet(const USet& other, Alloc alloc = Alloc());
    USet(USet&& other);

    // assign & move assign
    USet& operator=(const USet& rhs);
    USet& operator=(USet&& rhs);

    // compare
    bool operator==(const USet& rhs) const;
    bool operator!=(const USet& rhs) const;

    // getter
    SizeType        size() const;
    SizeType        capacity() const;
    SizeType        slack() const;
    SizeType        sparseSize() const;
    SizeType        holeSize() const;
    SizeType        bitArraySize() const;
    SizeType        freelistHead() const;
    SizeType        bucketSize() const;
    SizeType        bucketMask() const;
    bool            isCompact() const;
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
    bool compact();
    bool compactStable();
    bool compactTop();

    // data op
    KeyType&       keyOf(T& v) const;
    const KeyType& keyOf(const T& v) const;
    bool           keyEqual(const T& a, const T& b) const;
    HashType       hashOf(const T& v) const;

    // rehash
    bool needRehash();
    void rehash();
    bool rehashIfNeed();

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
    bool      _resizeBucket() const;
    SizeType  _bucketIndex(SizeType hash) const;
    SizeType& _bucketData(SizeType hash) const;
    DataInfo  _linkToBucketOrAssign(SizeType index, DataType& data);

private:
    mutable SizeType* m_bucket;
    mutable SizeType  m_bucket_size;
    mutable SizeType  m_bucket_mask;
    DataArr           m_data;
};
}// namespace kun

// USet impl
namespace kun
{
// helpers
template<typename T, typename Config, typename Alloc>
KUN_INLINE typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::_calcBucketSize(SizeType data_size) const
{
    static constexpr SizeType min_size_to_hash = 4;
    static constexpr SizeType basic_bucket_size = 8;
    static constexpr SizeType avg_bucket_capacity = 2;

    if (data_size >= min_size_to_hash)
    {
        return ceilPowerOf2(SizeType(data_size / avg_bucket_capacity) + basic_bucket_size);
    }
    else if (data_size)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
template<typename T, typename Config, typename Alloc> KUN_INLINE void USet<T, Config, Alloc>::_cleanBucket() const
{
    if (m_bucket)
    {
        SizeType* begin = m_bucket;
        SizeType* end = m_bucket + m_bucket_size;
        for (; begin != end; ++begin) { *begin = npos; }
    }
}
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::_resizeBucket() const
{
    SizeType new_bucket_size = _calcBucketSize(m_data.capacity());

    if (new_bucket_size != m_bucket_size)
    {
        if (new_bucket_size)
        {
            m_bucket = m_data.resizeContainer(m_bucket, m_bucket_size, m_bucket_size, new_bucket_size);
            m_bucket_size = new_bucket_size;
            m_bucket_mask = new_bucket_size - 1;
        }
        else
        {
            if (m_bucket)
            {
                m_data.allocator().free(m_bucket);
                m_bucket = nullptr;
                m_bucket_size = 0;
                m_bucket_mask = 0;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}
template<typename T, typename Config, typename Alloc>
KUN_INLINE typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::_bucketIndex(SizeType hash) const
{
    return hash & m_bucket_mask;
}
template<typename T, typename Config, typename Alloc>
KUN_INLINE typename USet<T, Config, Alloc>::SizeType& USet<T, Config, Alloc>::_bucketData(SizeType hash) const
{
    return m_bucket[_bucketIndex(hash)];
}
template<typename T, typename Config, typename Alloc>
KUN_INLINE typename USet<T, Config, Alloc>::DataInfo USet<T, Config, Alloc>::_linkToBucketOrAssign(SizeType index, DataType& data)
{
    DataInfo info(&data.data, index);
    if constexpr (!Config::multi_key)
    {
        // check if data has been added to set
        if (DataInfo& found_info = findHashed(keyOf(data.data), data.hash))
        {
            // cover the old data
            memory::moveItems(&found_info.data, &data.data, 1);

            // remove old data
            m_data.removeAt(index);

            // modify data
            info = found_info;
        }
        else
        {
            // link to bucket
            if (!rehashIfNeed())
            {
                SizeType& id_ref = _bucketData(data.hash);
                data.next = id_ref;
                id_ref = index;
            }
        }
    }
    else
    {
        // link to bucket
        if (!rehashIfNeed())
        {
            SizeType& id_ref = _bucketData(data.hash);
            data.next = id_ref;
            id_ref = index;
        }
    }
    return info;
}

// ctor & dtor
template<typename T, typename Config, typename Alloc>
KUN_INLINE USet<T, Config, Alloc>::USet(Alloc alloc)
    : m_bucket(nullptr)
    , m_bucket_size(0)
    , m_bucket_mask(0)
    , m_data(std::move(alloc))
{
}
template<typename T, typename Config, typename Alloc>
KUN_INLINE USet<T, Config, Alloc>::USet(SizeType reserve_size, Alloc alloc)
    : m_bucket(nullptr)
    , m_bucket_size(0)
    , m_bucket_mask(0)
    , m_data(std::move(alloc))
{
    reserve(reserve_size);
}
template<typename T, typename Config, typename Alloc>
KUN_INLINE USet<T, Config, Alloc>::USet(const T* p, SizeType n, Alloc alloc)
    : m_bucket(nullptr)
    , m_bucket_size(0)
    , m_bucket_mask(0)
    , m_data(std::move(alloc))
{
    append(p, n);
}
template<typename T, typename Config, typename Alloc>
KUN_INLINE USet<T, Config, Alloc>::USet(std::initializer_list<T> init_list, Alloc alloc)
    : m_bucket(nullptr)
    , m_bucket_size(0)
    , m_bucket_mask(0)
    , m_data(std::move(alloc))
{
    append(init_list);
}
template<typename T, typename Config, typename Alloc> KUN_INLINE USet<T, Config, Alloc>::~USet() { release(); }

// copy & move
template<typename T, typename Config, typename Alloc>
KUN_INLINE USet<T, Config, Alloc>::USet(const USet& other, Alloc alloc)
    : m_bucket(nullptr)
    , m_bucket_size(0)
    , m_bucket_mask(0)
    , m_data(std::move(alloc))
{
    *this = other;
}
template<typename T, typename Config, typename Alloc>
KUN_INLINE USet<T, Config, Alloc>::USet(USet&& other)
    : m_bucket(other.m_bucket)
    , m_bucket_size(other.m_bucket_size)
    , m_bucket_mask(other.m_bucket_mask)
    , m_data(std::move(other.m_data))
{
    other.m_bucket = nullptr;
    other.m_bucket_size = 0;
    other.m_bucket_mask = 0;
}

// assign & move assign
template<typename T, typename Config, typename Alloc> KUN_INLINE USet<T, Config, Alloc>& USet<T, Config, Alloc>::operator=(const USet& rhs)
{
    if (this != &rhs)
    {
        // clear
        clear();

        if (!rhs.empty())
        {
            // realloc bucket
            if (m_bucket_size != rhs.m_bucket_size)
            {
                m_bucket_size = rhs.m_bucket_size;
                m_bucket_mask = rhs.m_bucket_mask;
                m_bucket = m_data.allocator().template alloc<SizeType>(m_bucket_size);
            }

            // copy bucket
            memory::memcpy(m_bucket, rhs.m_bucket, m_bucket_size * sizeof(SizeType));

            // copy data
            m_data = rhs.m_data;
        }
    }
}
template<typename T, typename Config, typename Alloc> KUN_INLINE USet<T, Config, Alloc>& USet<T, Config, Alloc>::operator=(USet&& rhs)
{
    if (this != &rhs)
    {
        // clear
        clear();

        // move data
        m_bucket = rhs.m_bucket;
        m_bucket_size = rhs.m_bucket_size;
        m_bucket_mask = rhs.m_bucket_mask;
        m_data = std::move(rhs.m_data);

        // clean up rhs
        rhs.m_bucket = nullptr;
        rhs.m_bucket_size = 0;
        rhs.m_bucket_mask = 0;
    }
}

// compare
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::operator==(const USet& rhs) const
{
    if (size() == rhs.size())
    {
        return isSubsetOf(rhs);
    }
    else
    {
        return false;
    }
}
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::operator!=(const USet& rhs) const
{
    return !(*this == rhs);
}

// getter
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::size() const
{
    return m_data.size();
}
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::capacity() const
{
    return m_data.capacity();
}
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::slack() const
{
    return m_data.slack();
}
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::sparseSize() const
{
    return m_data.sparseSize();
}
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::holeSize() const
{
    return m_data.holeSize();
}
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::bitArraySize() const
{
    return m_data.bitArraySize();
}
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::freelistHead() const
{
    return m_data.freelistHead();
}
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::bucketSize() const
{
    return m_bucket_size;
}
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType USet<T, Config, Alloc>::bucketMask() const
{
    return m_bucket_mask;
}
template<typename T, typename Config, typename Alloc> bool USet<T, Config, Alloc>::isCompact() const { return m_data.isCompact(); }
template<typename T, typename Config, typename Alloc> bool USet<T, Config, Alloc>::empty() const { return m_data.empty(); }
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::DataArr& USet<T, Config, Alloc>::data() { return m_data; }
template<typename T, typename Config, typename Alloc> const typename USet<T, Config, Alloc>::DataArr& USet<T, Config, Alloc>::data() const
{
    return m_data;
}
template<typename T, typename Config, typename Alloc> typename USet<T, Config, Alloc>::SizeType* USet<T, Config, Alloc>::bucket() { return m_bucket; }
template<typename T, typename Config, typename Alloc> const typename USet<T, Config, Alloc>::SizeType* USet<T, Config, Alloc>::bucket() const
{
    return m_bucket;
}
template<typename T, typename Config, typename Alloc> Alloc&       USet<T, Config, Alloc>::allocator() { return m_data.allocator(); }
template<typename T, typename Config, typename Alloc> const Alloc& USet<T, Config, Alloc>::allocator() const { return m_data.allocator(); }

// validate
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::hasData(SizeType idx) const
{
    return m_data.hasData(idx);
}
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::isHole(SizeType idx) const
{
    return m_data.isHole(idx);
}
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::isValidIndex(SizeType idx) const
{
    return m_data.isValidIndex(idx);
}
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::isValidPointer(const T* p) const
{
    return m_data.isValidPointer(p);
}

// memory op
template<typename T, typename Config, typename Alloc> KUN_INLINE void USet<T, Config, Alloc>::clear()
{
    m_data.clear();
    _cleanBucket();
}
template<typename T, typename Config, typename Alloc> KUN_INLINE void USet<T, Config, Alloc>::release(SizeType capacity)
{
    m_data.release(capacity);
    _resizeBucket();
    _cleanBucket();
}
template<typename T, typename Config, typename Alloc> KUN_INLINE void USet<T, Config, Alloc>::reserve(SizeType capacity)
{
    m_data.reserve(capacity);
    rehashIfNeed();
}
template<typename T, typename Config, typename Alloc> KUN_INLINE void USet<T, Config, Alloc>::shrink()
{
    m_data.shrink();
    if (_resizeBucket())
    {
        rehash();
    }
}
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::compact()
{
    if (m_data.compact())
    {
        rehash();
        return true;
    }
    else
    {
        return false;
    }
}
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::compactStable()
{
    if (m_data.compactStable())
    {
        rehash();
        return true;
    }
    else
    {
        return false;
    }
}
template<typename T, typename Config, typename Alloc> KUN_INLINE bool USet<T, Config, Alloc>::compactTop() { return m_data.compactTop(); }
}// namespace kun
