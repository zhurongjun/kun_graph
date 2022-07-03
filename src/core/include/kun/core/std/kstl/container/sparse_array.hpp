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
// SparseArray data inf
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
    void _resizeBitArray();
    void _growBitArray();
    void _freeBitArray();

private:
    u32*            m_bit_array;
    SizeType        m_bit_array_size;
    SizeType        m_first_free_index;
    SizeType        m_num_free_indices;
    Array<DataType> m_data;
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
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_resizeBitArray()
{
    SizeType data_word_size = algo::calcNumWords(m_data.size());
    SizeType old_word_size = algo::calcNumWords(m_bit_array_size);

    if (data_word_size != old_word_size)
    {
        if (data_word_size > 0)
        {
            auto&    alloc = m_data.allocator();
            SizeType new_word_size = data_word_size;

            // resize
            alloc.resizeContainer(m_bit_array, old_word_size, old_word_size, new_word_size);

            // clean memory
            if (data_word_size > old_word_size)
            {
                algo::setWords(m_bit_array + old_word_size, new_word_size - old_word_size);
            }

            // update size
            m_bit_array_size = new_word_size;
        }
        else
        {
            _freeBitArray();
        }
    }
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_growBitArray()
{
    if (m_bit_array_size < m_data.size())
    {
        // calc grow size
        auto&    alloc = m_data.allocator();
        SizeType data_word_size = algo::calcNumWords(m_data.size());
        SizeType old_word_size = algo::calcNumWords(m_bit_array_size);
        SizeType new_word_size = alloc.getGrow(data_word_size, old_word_size);

        // alloc memory and clean
        if (new_word_size > old_word_size)
        {
            // alloc
            alloc.resizeContainer(m_bit_array, old_word_size, old_word_size, new_word_size);

            // clean
            algo::setWords(m_bit_array + old_word_size, new_word_size - old_word_size);

            // update size
            m_bit_array_size = new_word_size * algo::NumBitsPerDWORD;
        }
    }
}
template<typename T, typename Alloc> KUN_INLINE void SparseArray<T, Alloc>::_freeBitArray()
{
    if (m_bit_array)
    {
        m_data.allocator().free(m_bit_array);
        m_bit_array_size = 0;
    }
}
}// namespace kun