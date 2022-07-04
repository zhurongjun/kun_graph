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
    KUN_INLINE operator bool() { return data != nullptr; }
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
    using DataInfo = SparseArrayDataInfo<T, SizeType>;
    using ConstDataInfo = SparseArrayDataInfo<const T, SizeType>;

    // ctor & dtor
    SparseArray(Alloc alloc = Alloc());
    ~SparseArray();

    // copy & move
    SparseArray(const SparseArray& other, Alloc alloc = Alloc());
    SparseArray(SparseArray&& other);

    // assign & move assign
    SparseArray& operator=(const SparseArray& rhs);
    SparseArray& operator=(SparseArray&& rhs);

    // compare
    bool operator==(const SparseArray& rhs) const;
    bool operator!=(const SparseArray& rhs) const;

    // getter
    bool         isCompact() const;
    SizeType     size() const;
    SizeType     sparseSize() const;
    SizeType     holeSize() const;
    SizeType     capacity() const;
    SizeType     slack() const;
    Alloc&       allocator();
    const Alloc& allocator() const;
    bool         empty() const;

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

    // add
    DataInfo add(const T& v);
    DataInfo addUnsafe();
    DataInfo addDefault();
    DataInfo addZeroed();

    // add at
    void addAt(SizeType idx, const T& v);
    void addAtUnsafe(SizeType idx);
    void addAtDefault(SizeType idx);
    void addAtZeroed(SizeType idx);

    // emplace
    template<typename... Args> SizeType emplace(Args&&... args);
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
    template<typename TK> DataInfo      find(const TK& v);
    template<typename TK> DataInfo      findLast(const TK& v);
    template<typename TK> ConstDataInfo find(const TK& v) const;
    template<typename TK> ConstDataInfo findLast(const TK& v) const;

    // find if
    template<typename TP> DataInfo      findIf(TP&& p);
    template<typename TP> DataInfo      findLastIf(TP&& p);
    template<typename TP> ConstDataInfo findIf(TP&& p) const;
    template<typename TP> ConstDataInfo findLastIf(TP&& p) const;

    // contain
    template<typename TK> bool contain(const TK& v) const;
    template<typename TP> bool containIf(TP&& p) const;

    // sort
    template<typename TP = Less<T>> void sort(TP&& p = TP());
    template<typename TP = Less<T>> void sortStable(TP&& p = TP());

private:
    // helper
    void _setBit(SizeType index, bool v);
    bool _getBit(SizeType index);
    void _setBitRange(SizeType start, SizeType n, bool v);
    void _resizeMemory(SizeType new_capacity);
    void _grow(SizeType n);
    void _resizeBitArray();
    void _growBitArray();
    void _freeBitArray();

private:
    u32*      m_bit_array;
    SizeType  m_bit_array_size;
    SizeType  m_num_hole;
    SizeType  m_first_hole;
    SizeType  m_size;
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
template<typename T, typename Alloc> KUN_INLINE bool SparseArray<T, Alloc>::_getBit(SizeType index) { return algo::getBit(m_bit_array, index); }
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_setBitRange(SizeType start, SizeType n, bool v)
{
    algo::setBitRange(m_data, start, n, v);
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_resizeMemory(SizeType new_capacity)
{
    if (new_capacity)
    {
        // calc new size
        auto new_size = std::min(m_size, m_capacity);

        // realloc
        if constexpr (memory::memory_policy_traits<T>::use_realloc)
        {
            m_data = m_alloc.resizeContainer(m_data, m_size, m_capacity, new_capacity);
        }
        else
        {
            // alloc new memory
            DataType* new_memory = m_alloc.alloc<DataType>(new_capacity);

            // move items
            for (SizeType i = 0; i < new_size; ++i)
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
                for (SizeType i = 0; i < m_size; ++i)
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
        m_size = new_size;
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
                algo::setWords(m_bit_array + old_word_size, new_word_size - old_word_size);
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
            for (SizeType i = 0; i < m_size; ++i)
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
        m_first_hole = npos;
        m_size = 0;
        m_capacity = 0;
        m_data = nullptr;
    }
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_grow(SizeType n)
{
    auto new_size = m_size + n;

    // grow memory
    if (new_size > m_capacity)
    {
        auto new_capacity = m_alloc.getGrow(new_size, m_capacity);

        // grow memory
        if constexpr (memory::memory_policy_traits<T>::use_realloc)
        {
            m_data = m_alloc.resizeContainer(m_data, m_size, m_capacity, new_capacity);
            m_size = new_size;
        }
        else
        {
            // alloc new memory
            DataType* new_memory = m_alloc.alloc<DataType>(new_capacity);

            // move items
            for (SizeType i = 0; i < m_size; ++i)
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

            // update size
            m_size = new_size;
        }

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
                m_alloc.resizeContainer(m_bit_array, old_word_size, old_word_size, new_word_size);

                // clean
                algo::setWords(m_bit_array + old_word_size, new_word_size - old_word_size);

                // update size
                m_bit_array_size = new_word_size * algo::NumBitsPerDWORD;
            }
        }
    }
}

// ctor & dtor
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(Alloc alloc)
    : m_bit_array(nullptr)
    , m_bit_array_size(0)
    , m_num_hole(0)
    , m_first_hole(npos)
    , m_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc(std::move(alloc))
{
}
template<typename T, typename Alloc> KUN_INLINE SparseArray<T, Alloc>::~SparseArray() { release(); }

// copy & move
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(const SparseArray& other, Alloc alloc)
    : m_bit_array(nullptr)
    , m_bit_array_size(0)
    , m_num_hole(0)
    , m_first_hole(npos)
    , m_size(0)
    , m_capacity(0)
    , m_data(nullptr)
    , m_alloc(std::move(alloc))
{
    (*this) = other;
}
template<typename T, typename Alloc>
KUN_INLINE SparseArray<T, Alloc>::SparseArray(SparseArray&& other)
    : m_bit_array(other.m_bit_array)
    , m_bit_array_size(other.m_bit_array_size)
    , m_num_hole(other.m_num_hole)
    , m_first_hole(other.m_first_hole)
    , m_size(other.m_size)
    , m_capacity(other.m_capacity)
    , m_data(other.m_data)
    , m_alloc(std::move(other.m_alloc))
{
    other.m_bit_array = nullptr;
    other.m_bit_array_size = 0;
    other.m_num_hole = 0;
    other.m_first_hole = npos;
    other.m_size = 0;
    other.m_capacity = 0;
    other.m_data = nullptr;
}

// assign & move assign
template<typename T, typename Alloc> KUN_INLINE SparseArray<T, Alloc>& SparseArray<T, Alloc>::operator=(const SparseArray& rhs)
{
    if (this != &rhs)
    {
        clear();
        addUnsafe(rhs.size());

        // copy data
        if constexpr (memory::memory_policy_traits<T>::call_ctor)
        {
            for (SizeType i = 0; i < rhs.size(); ++i)
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
            memory::memcpy(m_data, rhs.m_data, sizeof(DataType) * rhs.m_size);
        }

        // copy bit array
        memory::memcpy(m_bit_array, rhs.m_bit_array, sizeof(u32) * (rhs.m_bit_array_size / algo::NumBitsPerDWORD));

        // copy other data
        m_bit_array_size = rhs.m_bit_array_size;
        m_num_hole = rhs.m_num_hole;
        m_first_hole = rhs.m_first_hole;
        m_size = rhs.m_size;
        m_capacity = rhs.m_capacity;
    }
}
template<typename T, typename Alloc> KUN_INLINE SparseArray<T, Alloc>& SparseArray<T, Alloc>::operator=(SparseArray&& rhs)
{
    if (this != &rhs)
    {
        release();

        // move data
        m_bit_array = rhs.m_bit_array;
        m_bit_array_size = rhs.m_bit_array_size;
        m_num_hole = rhs.m_num_hole;
        m_first_hole = rhs.m_first_hole;
        m_size = rhs.m_size;
        m_capacity = rhs.m_capacity;
        m_data = rhs.m_data;
        m_alloc = std::move(rhs.m_alloc);

        // invalidate rhs
        rhs.m_bit_array = nullptr;
        rhs.m_bit_array_size = 0;
        rhs.m_num_hole = 0;
        rhs.m_first_hole = npos;
        rhs.m_size = 0;
        rhs.m_capacity = 0;
        rhs.m_data = nullptr;
    }
}
}// namespace kun