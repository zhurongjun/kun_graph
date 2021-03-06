#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "sparse_array_iterator.hpp"
#include "kun/core/std/kstl/algo/intro_sort.hpp"
#include "kun/core/std/kstl/algo/merge_sort.hpp"
#include "kun/core/std/kstl/algo/remove.hpp"
#include "kun/core/std/kstl/algo/find.hpp"
#include "fwd.hpp"

// SparseArray def
namespace kun
{
template<typename T, typename Alloc> class SparseArray
{
public:
    using SizeType = typename Alloc::SizeType;
    using DataType = SparseArrayData<T, SizeType>;
    using DataInfo = SparseArrayDataInfo<T, SizeType>;
    using CDataInfo = SparseArrayDataInfo<const T, SizeType>;
    using It = SparseArrayIt<T, SizeType, false>;
    using CIt = SparseArrayIt<T, SizeType, true>;

    // ctor & dtor
    SparseArray(Alloc alloc = Alloc());
    SparseArray(SizeType size, Alloc alloc = Alloc());
    SparseArray(SizeType size, const T& v, Alloc alloc = Alloc());
    SparseArray(const T* p, SizeType n, Alloc alloc = Alloc());
    SparseArray(std::initializer_list<T> init_list, Alloc alloc = Alloc());
    ~SparseArray();

    // copy & move
    SparseArray(const SparseArray& other, Alloc alloc = Alloc());
    SparseArray(SparseArray&& other) noexcept;

    // assign & move assign
    SparseArray& operator=(const SparseArray& rhs);
    SparseArray& operator=(SparseArray&& rhs) noexcept;

    // special assign
    void assign(const T* p, SizeType n);
    void assign(std::initializer_list<T> init_list);

    // compare
    bool operator==(const SparseArray& rhs) const;
    bool operator!=(const SparseArray& rhs) const;

    // getter
    SizeType        size() const;
    SizeType        capacity() const;
    SizeType        slack() const;
    SizeType        sparseSize() const;
    SizeType        holeSize() const;
    SizeType        bitArraySize() const;
    SizeType        freelistHead() const;
    bool            isCompact() const;
    bool            empty() const;
    DataType*       data();
    const DataType* data() const;
    u32*            bitArray();
    const u32*      bitArray() const;
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

    // add
    DataInfo add(const T& v);
    DataInfo add(T&& v);
    DataInfo addUnsafe();
    DataInfo addDefault();
    DataInfo addZeroed();

    // add at
    void addAt(SizeType idx, const T& v);
    void addAt(SizeType idx, T&& v);
    void addAtUnsafe(SizeType idx);
    void addAtDefault(SizeType idx);
    void addAtZeroed(SizeType idx);

    // emplace
    template<typename... Args> DataInfo emplace(Args&&... args);
    template<typename... Args> void     emplaceAt(SizeType index, Args&&... args);

    // append
    void append(const SparseArray& arr);
    void append(std::initializer_list<T> init_list);
    void append(T* p, SizeType n);

    // remove
    void                           removeAt(SizeType index, SizeType n = 1);
    void                           removeAtUnsafe(SizeType index, SizeType n = 1);
    template<typename TK> SizeType remove(const TK& v);
    template<typename TK> SizeType removeLast(const TK& v);
    template<typename TK> SizeType removeAll(const TK& v);

    // remove if
    template<typename TP> SizeType removeIf(TP&& p);
    template<typename TP> SizeType removeLastIf(TP&& p);
    template<typename TP> SizeType removeAllIf(TP&& p);

    // modify
    T&       operator[](SizeType index);
    const T& operator[](SizeType index) const;

    // find
    template<typename TK> DataInfo  find(const TK& v);
    template<typename TK> DataInfo  findLast(const TK& v);
    template<typename TK> CDataInfo find(const TK& v) const;
    template<typename TK> CDataInfo findLast(const TK& v) const;

    // find if
    template<typename TP> DataInfo  findIf(TP&& p);
    template<typename TP> DataInfo  findLastIf(TP&& p);
    template<typename TP> CDataInfo findIf(TP&& p) const;
    template<typename TP> CDataInfo findLastIf(TP&& p) const;

    // contain
    template<typename TK> bool contain(const TK& v) const;
    template<typename TP> bool containIf(TP&& p) const;

    // sort
    template<typename TP = Less<T>> void sort(TP&& p = TP());
    template<typename TP = Less<T>> void sortStable(TP&& p = TP());

    // support foreach
    It  begin();
    It  end();
    CIt begin() const;
    CIt end() const;

private:
    // helper
    void _setBit(SizeType index, bool v);
    bool _getBit(SizeType index) const;
    void _setBitRange(SizeType start, SizeType n, bool v);
    void _resizeMemory(SizeType new_capacity);
    void _grow(SizeType n);

private:
    u32*      m_bit_array;
    SizeType  m_bit_array_size;
    SizeType  m_num_hole;
    SizeType  m_freelist_head;
    SizeType  m_sparse_size;
    SizeType  m_capacity;
    DataType* m_data;
    Alloc     m_alloc;
};
}// namespace kun

// SparseArray impl
namespace kun
{
// helper
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_setBit(SizeType index, bool v) { algo::setBit(m_bit_array, index, v); }
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::_getBit(SizeType index) const { return algo::getBit(m_bit_array, index); }
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_setBitRange(SizeType start, SizeType n, bool v)
{
    algo::setBitRange(m_bit_array, start, n, v);
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_resizeMemory(SizeType new_capacity)
{
    if (new_capacity)
    {
        // calc new size
        auto new_sparse_size = std::min(m_sparse_size, m_capacity);

        // realloc
        if constexpr (memory::memory_policy_traits<T>::use_realloc)
        {
            m_data = m_alloc.resizeContainer(m_data, m_sparse_size, m_capacity, new_capacity);
        }
        else
        {
            // alloc new memory
            DataType* new_memory = m_alloc.template alloc<DataType>(new_capacity);

            // move items
            for (SizeType i = 0; i < new_sparse_size; ++i)
            {
                DataType& new_data = *(new_memory + i);
                DataType& old_data = *(m_data + i);
                if (hasData(i))
                {
                    new_data.data = std::move(old_data.data);
                }
                else
                {
                    new_data.prev = old_data.prev;
                    new_data.next = old_data.next;
                }
            }

            // destruct items
            if constexpr (memory::memory_policy_traits<T>::call_dtor)
            {
                for (SizeType i = 0; i < m_sparse_size; ++i)
                {
                    if (hasData(i))
                    {
                        (m_data + i)->data.~T();
                    }
                }
            }

            // release old memory
            m_alloc.free(m_data);
            m_data = new_memory;
        }

        // update size
        m_sparse_size = new_sparse_size;
        m_capacity = new_capacity;

        // resize bit array
        SizeType data_word_size = algo::calcNumWords(m_capacity);
        SizeType old_word_size = algo::calcNumWords(m_bit_array_size);
        if (data_word_size != old_word_size)
        {
            SizeType new_word_size = data_word_size;

            // resize
            m_bit_array = m_alloc.resizeContainer(m_bit_array, old_word_size, old_word_size, new_word_size);

            // clean memory
            if (data_word_size > old_word_size)
            {
                algo::setWords(m_bit_array + old_word_size, new_word_size - old_word_size, false);
            }

            // update size
            m_bit_array_size = new_word_size * algo::NumBitsPerDWORD;
        }
    }
    else if (m_data)
    {
        // destruct items
        if constexpr (memory::memory_policy_traits<T>::call_dtor)
        {
            for (SizeType i = 0; i < m_sparse_size; ++i)
            {
                if (hasData(i))
                {
                    (m_data + i)->data.~T();
                }
            }
        }

        // free memory
        m_alloc.free(m_data);
        m_alloc.free(m_bit_array);

        // reset data
        m_bit_array = nullptr;
        m_bit_array_size = 0;
        m_num_hole = 0;
        m_freelist_head = npos;
        m_sparse_size = 0;
        m_capacity = 0;
        m_data = nullptr;
    }
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_grow(SizeType n)
{
    auto new_sparse_size = m_sparse_size + n;

    // grow memory
    if (new_sparse_size > m_capacity)
    {
        auto new_capacity = m_alloc.getGrow(new_sparse_size, m_capacity);

        // grow memory
        if constexpr (memory::memory_policy_traits<T>::use_realloc)
        {
            m_data = m_alloc.resizeContainer(m_data, m_sparse_size, m_capacity, new_capacity);
        }
        else
        {
            // alloc new memory
            DataType* new_memory = m_alloc.template alloc<DataType>(new_capacity);

            // move items
            for (SizeType i = 0; i < m_sparse_size; ++i)
            {
                DataType& new_data = *(new_memory + i);
                DataType& old_data = *(m_data + i);
                if (hasData(i))
                {
                    new_data.data = std::move(old_data.data);
                    if constexpr (memory::memory_policy_traits<T>::call_dtor)
                    {
                        old_data->data.~T();
                    }
                }
                else
                {
                    new_data.prev = old_data.prev;
                    new_data.next = old_data.next;
                }
            }

            // release old memory
            m_alloc.free(m_data);
            m_data = new_memory;
        }

        // update capacity
        m_capacity = new_capacity;

        // grow bit array
        if (m_bit_array_size < m_capacity)
        {
            // calc grow size
            SizeType data_word_size = algo::calcNumWords(m_capacity);
            SizeType old_word_size = algo::calcNumWords(m_bit_array_size);
            SizeType new_word_size = m_alloc.getGrow(data_word_size, old_word_size);

            // alloc memory and clean
            if (new_word_size > old_word_size)
            {
                // alloc
                m_bit_array = m_alloc.resizeContainer(m_bit_array, old_word_size, old_word_size, new_word_size);

                // clean
                algo::setWords(m_bit_array + old_word_size, new_word_size - old_word_size, false);

                // update size
                m_bit_array_size = new_word_size * algo::NumBitsPerDWORD;
            }
        }
    }

    // update size
    m_sparse_size = new_sparse_size;
}

// ctor & dtor
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(Alloc alloc)
    : m_bit_array(nullptr)
    , m_bit_array_size(0)
    , m_num_hole(0)
    , m_freelist_head(npos)
    , m_sparse_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc(std::move(alloc))
{
}
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(SizeType size, Alloc alloc)
    : m_bit_array(nullptr)
    , m_bit_array_size(0)
    , m_num_hole(0)
    , m_freelist_head(npos)
    , m_sparse_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc(std::move(alloc))
{
    if (size)
    {
        // resize
        _resizeMemory(size);
        m_sparse_size = size;
        _setBitRange(0, size, true);

        // call ctor
        for (SizeType i = 0; i < size; ++i) { new (&m_data[i].data) T(); }
    }
}
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(SizeType size, const T& v, Alloc alloc)
    : m_bit_array(nullptr)
    , m_bit_array_size(0)
    , m_num_hole(0)
    , m_freelist_head(npos)
    , m_sparse_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc(std::move(alloc))
{
    if (size)
    {
        // resize
        _resizeMemory(size);
        m_sparse_size = size;
        _setBitRange(0, size, true);

        // call ctor
        for (SizeType i = 0; i < size; ++i) { new (&m_data[i].data) T(v); }
    }
}
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(const T* p, SizeType n, Alloc alloc)
    : m_bit_array(nullptr)
    , m_bit_array_size(0)
    , m_num_hole(0)
    , m_freelist_head(npos)
    , m_sparse_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc(std::move(alloc))
{
    if (n)
    {
        // resize
        _resizeMemory(n);
        m_sparse_size = n;
        _setBitRange(0, n, true);

        // call ctor
        for (SizeType i = 0; i < n; ++i) { new (&m_data[i].data) T(p[i]); }
    }
}
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(std::initializer_list<T> init_list, Alloc alloc)
    : m_bit_array(nullptr)
    , m_bit_array_size(0)
    , m_num_hole(0)
    , m_freelist_head(npos)
    , m_sparse_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc(std::move(alloc))
{
    SizeType size = init_list.size();
    if (size)
    {
        // resize
        _resizeMemory(size);
        m_sparse_size = size;
        _setBitRange(0, size, true);

        // call ctor
        for (SizeType i = 0; i < size; ++i) { new (&m_data[i].data) T(*(init_list.begin() + i)); }
    }
}
template<typename T, typename Alloc> KUN_INLINE SparseArray<T, Alloc>::~SparseArray() { release(); }

// copy & move
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(const SparseArray& other, Alloc alloc)
    : m_bit_array(nullptr)
    , m_bit_array_size(0)
    , m_num_hole(0)
    , m_freelist_head(npos)
    , m_sparse_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc(std::move(alloc))
{
    (*this) = other;
}
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(SparseArray&& other) noexcept
    : m_bit_array(other.m_bit_array)
    , m_bit_array_size(other.m_bit_array_size)
    , m_num_hole(other.m_num_hole)
    , m_freelist_head(other.m_freelist_head)
    , m_sparse_size(other.m_sparse_size)
    , m_capacity(other.m_capacity)
    , m_data(other.m_data)
    , m_alloc(std::move(other.m_alloc))
{
    other.m_bit_array = nullptr;
    other.m_bit_array_size = 0;
    other.m_num_hole = 0;
    other.m_freelist_head = npos;
    other.m_sparse_size = 0;
    other.m_capacity = 0;
    other.m_data = nullptr;
}

// assign & move assign
template<typename T, typename Alloc> KUN_INLINE SparseArray<T, Alloc>& SparseArray<T, Alloc>::operator=(const SparseArray& rhs)
{
    if (this != &rhs)
    {
        clear();
        reserve(rhs.m_sparse_size);
        m_sparse_size = rhs.m_sparse_size;

        // copy data
        if constexpr (memory::memory_policy_traits<T>::call_ctor)
        {
            for (SizeType i = 0; i < rhs.m_sparse_size; ++i)
            {
                DataType*       dst_data = m_data + i;
                const DataType* src_data = rhs.m_data + i;

                if (rhs.hasData(i))
                {
                    new (dst_data) T(*src_data);
                }
                else
                {
                    dst_data->prev = src_data->prev;
                    dst_data->next = src_data->next;
                }
            }
        }
        else
        {
            // copy data
            memory::memcpy(m_data, rhs.m_data, sizeof(DataType) * rhs.m_sparse_size);
        }

        // copy bit array
        memory::memcpy(m_bit_array, rhs.m_bit_array, sizeof(u32) * algo::calcNumWords(rhs.m_sparse_size));

        // copy other data
        m_num_hole = rhs.m_num_hole;
        m_freelist_head = rhs.m_freelist_head;
    }
    return *this;
}
template<typename T, typename Alloc> KUN_INLINE SparseArray<T, Alloc>& SparseArray<T, Alloc>::operator=(SparseArray&& rhs) noexcept
{
    if (this != &rhs)
    {
        release();

        // move data
        m_bit_array = rhs.m_bit_array;
        m_bit_array_size = rhs.m_bit_array_size;
        m_num_hole = rhs.m_num_hole;
        m_freelist_head = rhs.m_freelist_head;
        m_sparse_size = rhs.m_sparse_size;
        m_capacity = rhs.m_capacity;
        m_data = rhs.m_data;
        m_alloc = std::move(rhs.m_alloc);

        // invalidate rhs
        rhs.m_bit_array = nullptr;
        rhs.m_bit_array_size = 0;
        rhs.m_num_hole = 0;
        rhs.m_freelist_head = npos;
        rhs.m_sparse_size = 0;
        rhs.m_capacity = 0;
        rhs.m_data = nullptr;
    }
    return *this;
}

// special assign
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::assign(const T* p, SizeType n)
{
    clear();

    if (n)
    {
        // resize
        reserve(n);
        m_sparse_size = n;
        _setBitRange(0, n, true);

        // call ctor
        for (SizeType i = 0; i < n; ++i) { new (&m_data[i].data) T(p[i]); }
    }
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::assign(std::initializer_list<T> init_list)
{
    clear();

    SizeType size = init_list.size();
    if (size)
    {
        // resize
        reserve(size);
        m_sparse_size = size;
        _setBitRange(0, size, true);

        // call ctor
        for (SizeType i = 0; i < size; ++i) { new (&m_data[i].data) T(*(init_list.begin() + i)); }
    }
}

// compare
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::operator==(const SparseArray& rhs) const
{
    if (m_sparse_size == rhs.m_sparse_size)
    {
        for (SizeType i = 0; i < m_sparse_size; ++i)
        {
            bool lhs_has_data = hasData(i);
            bool rhs_has_data = rhs.hasData(i);

            if (lhs_has_data != rhs_has_data)
            {
                return false;
            }
            else if (lhs_has_data)
            {
                if ((*this)[i] != rhs[i])
                {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::operator!=(const SparseArray& rhs) const { return !((*this) == rhs); }

// getter
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::size() const
{
    return m_sparse_size - m_num_hole;
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::capacity() const
{
    return m_capacity;
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::slack() const
{
    return m_capacity - m_sparse_size + m_num_hole;
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::sparseSize() const
{
    return m_sparse_size;
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::holeSize() const
{
    return m_num_hole;
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::bitArraySize() const
{
    return m_bit_array_size;
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::freelistHead() const
{
    return m_freelist_head;
}
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::isCompact() const { return m_num_hole == 0; }
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::empty() const { return (m_sparse_size - m_num_hole) == 0; }
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::DataType*       SparseArray<T, Alloc>::data() { return m_data; }
template<typename T, typename Alloc> KUN_INLINE const typename SparseArray<T, Alloc>::DataType* SparseArray<T, Alloc>::data() const { return m_data; }
template<typename T, typename Alloc> KUN_INLINE u32*         SparseArray<T, Alloc>::bitArray() { return m_bit_array; }
template<typename T, typename Alloc> KUN_INLINE const u32*   SparseArray<T, Alloc>::bitArray() const { return m_bit_array; }
template<typename T, typename Alloc> KUN_INLINE Alloc&       SparseArray<T, Alloc>::allocator() { return m_alloc; }
template<typename T, typename Alloc> KUN_INLINE const Alloc& SparseArray<T, Alloc>::allocator() const { return m_alloc; }

// validate
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::hasData(SizeType idx) const { return _getBit(idx); }
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::isHole(SizeType idx) const { return !_getBit(idx); }
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::isValidIndex(SizeType idx) const
{
    return idx >= 0 && idx < m_sparse_size;
}
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::isValidPointer(const T* p) const
{
    return p >= reinterpret_cast<T*>(m_data) && p < reinterpret_cast<T*>(m_data + m_sparse_size);
}

// memory op
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::clear()
{
    // destruct items
    if constexpr (memory::memory_policy_traits<T>::call_dtor)
    {
        for (SizeType i = 0; i < m_sparse_size; ++i)
        {
            if (hasData(i))
            {
                (m_data + i)->data.~T();
            }
        }
    }

    // clean up bit array
    if (m_bit_array)
    {
        algo::setWords(m_bit_array, algo::calcNumWords(m_bit_array_size), false);
    }

    // clean up data
    m_num_hole = 0;
    m_freelist_head = npos;
    m_sparse_size = 0;
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::release(SizeType capacity)
{
    clear();
    _resizeMemory(capacity);
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::reserve(SizeType capacity)
{
    if (capacity > m_capacity)
    {
        _resizeMemory(capacity);
    }
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::shrink()
{
    auto new_capacity = m_alloc.getShrink(m_sparse_size, m_capacity);
    _resizeMemory(new_capacity);
}
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::compact()
{
    if (!isCompact())
    {
        // fill hole
        SizeType compacted_index = m_sparse_size - m_num_hole;
        SizeType search_index = m_sparse_size;
        while (m_freelist_head != npos)
        {
            SizeType next_index = (m_data + m_freelist_head)->next;
            if (m_freelist_head < compacted_index)
            {
                // find last allocated element
                do {
                    --search_index;
                } while (!hasData(search_index));

                // move element to the hole
                *(m_data + m_freelist_head) = std::move(*(m_data + search_index));
                (m_data + search_index)->data.~T();
            }
            m_freelist_head = next_index;
        }

        // setup bit array
        _setBitRange(compacted_index, m_num_hole, false);

        // setup data
        m_num_hole = 0;
        m_sparse_size = compacted_index;

        return true;
    }
    else
    {
        return false;
    }
}
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::compactStable()
{
    if (!isCompact())
    {
        SizeType compacted_index = m_sparse_size - m_num_hole;
        SizeType read_index = 0;
        SizeType write_index = 0;

        // skip first compacted range
        while (hasData(write_index) && write_index != m_sparse_size) ++write_index;
        read_index = write_index + 1;

        // copy items
        while (read_index < m_sparse_size)
        {
            // skip hole
            while (!hasData(read_index) && read_index < m_sparse_size) ++read_index;

            // move items
            while (read_index < m_sparse_size && hasData(read_index))
            {
                (m_data + write_index)->data = std::move((m_data + read_index)->data);
                (m_data + read_index)->data.~T();
                _setBit(write_index, true);
                ++write_index;
                ++read_index;
            }
        }

        // reset data
        _setBitRange(compacted_index, m_num_hole, false);
        m_num_hole = 0;
        m_freelist_head = npos;
        m_sparse_size = compacted_index;

        return true;
    }
    else
    {
        return false;
    }
}
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::compactTop()
{
    if (!isCompact())
    {
        bool has_changes = false;
        for (SizeType i(m_sparse_size - 1), n(m_sparse_size); n; --i, --n)
        {
            DataType& data = m_data[i];

            if (isHole(i))
            {
                // remove from freelist
                --m_num_hole;
                if (m_freelist_head == i)
                {
                    m_freelist_head = data.next;
                }
                if (data.next != npos)
                {
                    m_data[data.next].prev = data.prev;
                }
                if (data.prev != npos)
                {
                    m_data[data.prev].next = data.next;
                }

                // update size
                --m_sparse_size;

                has_changes = true;
            }
            else
            {
                break;
            }
        }
        return has_changes;
    }
    else
    {
        return false;
    }
}

// add
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::add(const T& v)
{
    DataInfo info = addUnsafe();
    new (info.data) T(v);
    return info;
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::add(T&& v)
{
    DataInfo info = addUnsafe();
    new (info.data) T(std::move(v));
    return info;
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::addUnsafe()
{
    SizeType index;

    if (m_num_hole)
    {
        // remove and use first index from freelist
        index = m_freelist_head;
        m_freelist_head = m_data[m_freelist_head].next;
        --m_num_hole;

        // break link
        if (m_num_hole)
        {
            m_data[m_freelist_head].prev = npos;
        }
    }
    else
    {
        // add new element
        index = m_sparse_size;
        _grow(1);
    }

    // setup bit
    _setBit(index, true);

    return DataInfo(&m_data[index].data, index);
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::addDefault()
{
    DataInfo info = addUnsafe();
    new (info.data) T();
    return info;
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::addZeroed()
{
    DataInfo info = addUnsafe();
    memory::memzero(info.data, sizeof(T));
    return info;
}

// add at
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::addAt(SizeType idx, const T& v)
{
    addAtUnsafe(idx);
    new (&m_data[idx].data) T(v);
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::addAt(SizeType idx, T&& v)
{
    addAtUnsafe(idx);
    new (&m_data[idx].data) T(std::move(v));
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::addAtUnsafe(SizeType idx)
{
    KUN_Assert(isHole(idx));
    KUN_Assert(isValidIndex(idx));

    DataType& data = m_data[idx];

    // remove from freelist
    --m_num_hole;
    if (m_freelist_head == idx)
    {
        m_freelist_head = data.next;
    }
    if (data.next != npos)
    {
        m_data[data.next].prev = data.prev;
    }
    if (data.prev != npos)
    {
        m_data[data.prev].next = data.next;
    }

    // setup bit
    _setBit(idx, true);
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::addAtDefault(SizeType idx)
{
    addAtUnsafe(idx);
    new (&m_data[idx].data) T();
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::addAtZeroed(SizeType idx)
{
    addAtUnsafe(idx);
    memory::memzero(&m_data[idx].data, sizeof(T));
}

// emplace
template<typename T, typename Alloc>
template<typename... Args>
KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::emplace(Args&&... args)
{
    DataInfo info = addUnsafe();
    new (info.data) T(std::forward<Args>(args)...);
    return info;
}
template<typename T, typename Alloc> template<typename... Args> KUN_INLINE void SparseArray<T, Alloc>::emplaceAt(SizeType index, Args&&... args)
{
    addAtUnsafe(index);
    new (&m_data[index].data) T(std::forward<Args>(args)...);
}

// append
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::append(const SparseArray& arr)
{
    for (const T& data : arr) { add(data); }
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::append(std::initializer_list<T> init_list)
{
    for (const T& data : init_list) { add(data); }
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::append(T* p, SizeType n)
{
    for (SizeType i = 0; i < n; ++i) { add(p[i]); }
}

// remove
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::removeAt(SizeType index, SizeType n)
{
    KUN_Assert(isValidIndex(index));
    KUN_Assert(isValidIndex(index + n - 1));
    KUN_Assert(n > 0);

    if constexpr (memory::memory_policy_traits<T>::call_dtor)
    {
        for (SizeType i = 0; i < n; ++i) { m_data[index + i].data.~T(); }
    }

    removeAtUnsafe(index, n);
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::removeAtUnsafe(SizeType index, SizeType n)
{
    KUN_Assert(isValidIndex(index));
    KUN_Assert(isValidIndex(index + n - 1));
    KUN_Assert(n > 0);

    for (; n; --n)
    {
        DataType& data = m_data[index];

        // link to freelist
        if (m_num_hole)
        {
            m_data[m_freelist_head].prev = index;
        }
        data.prev = npos;
        data.next = m_freelist_head;
        m_freelist_head = index;
        ++m_num_hole;

        // set flag
        _setBit(index, false);

        // update index
        ++index;
    }
}
template<typename T, typename Alloc>
template<typename TK>
KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::remove(const TK& v)
{
    return removeIf([&v](const T& a) { return a == v; });
}
template<typename T, typename Alloc>
template<typename TK>
KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::removeLast(const TK& v)
{
    return removeLastIf([&v](const T& a) { return a == v; });
}
template<typename T, typename Alloc>
template<typename TK>
KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::removeAll(const TK& v)
{
    return removeAllIf([&v](const T& a) { return a == v; });
}

// remove if
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::removeIf(TP&& p)
{
    if (DataInfo info = findIf(std::forward<TP>(p)))
    {
        removeAt(info.index);
        return info.index;
    }
    return npos;
}
template<typename T, typename Alloc>
template<typename TP>
KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::removeLastIf(TP&& p)
{
    if (DataInfo info = findLastIf(std::forward<TP>(p)))
    {
        removeAt(info.index);
        return info.index;
    }
    return npos;
}
template<typename T, typename Alloc>
template<typename TP>
KUN_INLINE typename SparseArray<T, Alloc>::SizeType SparseArray<T, Alloc>::removeAllIf(TP&& p)
{
    SizeType count = 0;
    for (SizeType i = 0; i < m_sparse_size; ++i)
    {
        if (hasData(i) && p(m_data[i].data))
        {
            removeAt(i);
            ++count;
        }
    }
    return count;
}

// modify
template<typename T, typename Alloc> KUN_INLINE T& SparseArray<T, Alloc>::operator[](SizeType index)
{
    KUN_Assert(isValidIndex(index));
    return m_data[index].data;
}
template<typename T, typename Alloc> KUN_INLINE const T& SparseArray<T, Alloc>::operator[](SizeType index) const
{
    KUN_Assert(isValidIndex(index));
    return m_data[index].data;
}

// find
template<typename T, typename Alloc>
template<typename TK>
KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::find(const TK& v)
{
    return findIf([&v](const T& a) { return a == v; });
}
template<typename T, typename Alloc>
template<typename TK>
KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::findLast(const TK& v)
{
    return findLastIf([&v](const T& a) { return a == v; });
}
template<typename T, typename Alloc>
template<typename TK>
KUN_INLINE typename SparseArray<T, Alloc>::CDataInfo SparseArray<T, Alloc>::find(const TK& v) const
{
    return findIf([&v](const T& a) { return a == v; });
}
template<typename T, typename Alloc>
template<typename TK>
KUN_INLINE typename SparseArray<T, Alloc>::CDataInfo SparseArray<T, Alloc>::findLast(const TK& v) const
{
    return findLastIf([&v](const T& a) { return a == v; });
}

// find if
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::findIf(TP&& p)
{
    for (SizeType i = 0; i < m_sparse_size; ++i)
    {
        if (hasData(i))
        {
            auto& data = m_data[i].data;
            if (p(data))
            {
                return DataInfo(&data, i);
            }
        }
    }
    return DataInfo();
}
template<typename T, typename Alloc>
template<typename TP>
KUN_INLINE typename SparseArray<T, Alloc>::DataInfo SparseArray<T, Alloc>::findLastIf(TP&& p)
{
    for (SizeType i(m_sparse_size - 1), n(m_sparse_size); n; --i, --n)
    {
        if (hasData(i))
        {
            auto& data = m_data[i].data;
            if (p(data))
            {
                return DataInfo(&data, i);
            }
        }
    }
    return DataInfo();
}
template<typename T, typename Alloc>
template<typename TP>
KUN_INLINE typename SparseArray<T, Alloc>::CDataInfo SparseArray<T, Alloc>::findIf(TP&& p) const
{
    for (SizeType i = 0; i < m_sparse_size; ++i)
    {
        if (hasData(i))
        {
            auto& data = m_data[i].data;
            if (p(data))
            {
                return CDataInfo(&data, i);
            }
        }
    }
    return CDataInfo();
}
template<typename T, typename Alloc>
template<typename TP>
KUN_INLINE typename SparseArray<T, Alloc>::CDataInfo SparseArray<T, Alloc>::findLastIf(TP&& p) const
{
    for (SizeType i(m_sparse_size - 1), n(m_sparse_size); n; --i, --n)
    {
        if (hasData(i))
        {
            auto& data = m_data[i].data;
            if (p(data))
            {
                return CDataInfo(&data, i);
            }
        }
    }
    return CDataInfo();
}

// contain
template<typename T, typename Alloc> template<typename TK> KUN_INLINE bool SparseArray<T, Alloc>::contain(const TK& v) const { return (bool)find(v); }
template<typename T, typename Alloc> template<typename TP> KUN_INLINE bool SparseArray<T, Alloc>::containIf(TP&& p) const
{
    return (bool)findIf(std::forward<TP>(p));
}

// sort
template<typename T, typename Alloc> template<typename TP> KUN_INLINE void SparseArray<T, Alloc>::sort(TP&& p)
{
    if (m_sparse_size)
    {
        compact();
        algo::introSort(m_data, m_data + m_sparse_size, [&p](const DataType& a, const DataType& b) { return p(a.data, b.data); });
    }
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE void SparseArray<T, Alloc>::sortStable(TP&& p)
{
    if (m_sparse_size)
    {
        compactStable();
        algo::mergeSort(m_data, m_data + m_sparse_size, [&p](const DataType& a, const DataType& b) { return p(a.data, b.data); });
    }
}

// support foreach
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::It SparseArray<T, Alloc>::begin()
{
    return It(m_data, m_sparse_size, m_bit_array);
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::It SparseArray<T, Alloc>::end()
{
    return It(m_data, m_sparse_size, m_bit_array, m_sparse_size);
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::CIt SparseArray<T, Alloc>::begin() const
{
    return CIt(m_data, m_sparse_size, m_bit_array);
}
template<typename T, typename Alloc> KUN_INLINE typename SparseArray<T, Alloc>::CIt SparseArray<T, Alloc>::end() const
{
    return CIt(m_data, m_sparse_size, m_bit_array, m_sparse_size);
}
}// namespace kun