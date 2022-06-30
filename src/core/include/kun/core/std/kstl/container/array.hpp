#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/algo/intro_sort.hpp"
#include "kun/core/std/kstl/algo/merge_sort.hpp"
#include "fwd.hpp"

// Array def
namespace kun
{
// TODO. consider support range
template<typename T, typename Alloc> class Array
{
public:
    using SizeType = typename Alloc::USizeType;

    // ctor & dtor
    Array(Alloc alloc = Alloc());
    Array(SizeType size, Alloc alloc = Alloc());
    Array(SizeType size, const T& v, Alloc alloc = Alloc());
    Array(const T* p, SizeType n, Alloc alloc = Alloc());
    Array(std::initializer_list<T> init_list, Alloc alloc = Alloc());
    ~Array();

    // copy & move
    Array(const Array& other, Alloc alloc = Alloc());
    Array(Array&& other);

    // assign & move assign
    Array& operator=(const Array& rhs);
    Array& operator=(Array&& rhs);

    // compare
    bool operator==(const Array& rhs) const;
    bool operator!=(const Array& rhs) const;

    // getter
    T*           data();
    const T*     data() const;
    SizeType     size() const;
    SizeType     capacity() const;
    SizeType     slack() const;
    Alloc&       allocator();
    const Alloc& allocator() const;
    bool         empty();

    // validate
    bool isValidIndex(SizeType idx) const;
    bool isValidPointer(const T* p) const;

    // memory op
    void clear();
    void release(SizeType capacity = 0);
    void reserve(SizeType capacity);
    void shrink();
    void resize(SizeType size, const T& new_value);
    void resizeUnsafe(SizeType size);
    void resizeZeroed(SizeType size);
    void resizeDefault(SizeType size);

    // add
    SizeType add(const T& v);
    SizeType add(T&& v);
    SizeType add(const T& v, SizeType n);
    SizeType addUnique(const T& v);
    SizeType addUnsafe(SizeType n = 1);
    SizeType addZeroed(SizeType n = 1);
    SizeType addDefault(SizeType n = 1);
    void     addAt(SizeType idx, const T& v);
    void     addAt(SizeType idx, T&& v);
    void     addAt(SizeType idx, const Array& arr);
    void     addAt(SizeType idx, std::initializer_list<T> init_list);
    void     addAt(SizeType idx, T* p, SizeType n);
    void     addAtUnsafe(SizeType idx, SizeType n = 1);
    void     addAtZeroed(SizeType idx, SizeType n = 1);
    SizeType addAtDefault(SizeType n = 1);

    // emplace
    template<typename... Args> SizeType emplace(Args&&... args);
    template<typename... Args> SizeType emplaceAt(SizeType index, Args&&... args);

    // append
    SizeType append(const Array& other);
    SizeType append(Array&& other);
    SizeType append(std::initializer_list<T> other);
    SizeType append(T* p, SizeType n);

    // assign
    void assign(T* p, SizeType n);
    void assign(std::initializer_list<T> other);

    // remove
    void     removeAt(SizeType index, SizeType n = 1);
    void     removeAtSwap(SizeType index, SizeType n = 1);
    SizeType remove(const T& v);
    SizeType removeSwap(const T& v);
    SizeType removeLast(const T& v);
    SizeType removeLastSwap(const T& v);
    SizeType removeAll(const T& v);
    SizeType removeAllSwap(const T& v);

    // remove by
    template<typename TP> SizeType removeBy(TP&& p);
    template<typename TP> SizeType removeBySwap(TP&& p);
    template<typename TP> SizeType removeLastBy(TP&& p);
    template<typename TP> SizeType removeLastBySwap(TP&& p);
    template<typename TP> SizeType removeAllBy(TP&& p);
    template<typename TP> SizeType removeAllBySwap(TP&& p);

    // modify
    T&       operator[](SizeType index);
    const T& operator[](SizeType index) const;

    // find
    T*       find(const T& v);
    T*       findLast(const T& v);
    const T* find(const T& v) const;
    const T* findLast(const T& v) const;

    // find by
    template<typename TP> T*       findBy(TP&& p);
    template<typename TP> T*       findLastBy(TP&& p);
    template<typename TP> const T* findBy(TP&& p) const;
    template<typename TP> const T* findLastBy(TP&& p) const;

    // contain
    bool contain(const T& v) const;

    // contain by
    template<typename TP> bool containBy(TP&& p) const;

    // sort
    template<typename TP = Less<T>> void sort(TP&& p = TP());
    template<typename TP = Less<T>> void sortStable(TP&& p = TP());

    // support heap
    T&                                       heapTop();
    template<typename TP = Less<T>> void     heapify(TP&& p = TP());
    template<typename TP = Less<T>> bool     isHeap(TP&& p = TP());
    template<typename TP = Less<T>> SizeType heapPush(T&& v, TP&& p = TP());
    template<typename TP = Less<T>> SizeType heapPush(const T& v, TP&& p = TP());
    template<typename TP = Less<T>> bool     heapPop(TP&& p = TP());
    template<typename TP = Less<T>> T        heapPopGet(T& out, TP&& p = TP());
    template<typename TP = Less<T>> bool     heapRemoveAt(SizeType index, TP&& p = TP());
    template<typename TP = Less<T>> bool     heapSort(TP&& p = TP());

    // support stack
    void     pop();
    T        popGet();
    void     push(const T& v);
    void     push(T&& v);
    T&       top();
    const T& top() const;
    T&       bottom();
    const T& bottom() const;

    // support foreach
    T*       begin();
    T*       end();
    const T* begin() const;
    const T* end() const;

private:
    // helper
    void _resizeMemory(SizeType new_capacity);
    void _grow(SizeType n);

private:
    T*       m_data;
    SizeType m_size;
    SizeType m_capacity;
    Alloc    m_alloc;
};
}// namespace kun

// Array impl
namespace kun
{
// helper
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::_resizeMemory(SizeType new_capacity)
{
    if (new_capacity)
    {
        // realloc
        m_data = m_alloc.resizeContainer(m_data, m_size, m_capacity, new_capacity);
        m_size = std::min(m_size, m_capacity);
        m_capacity = new_capacity;
    }
    else if (m_data)
    {
        // free
        memory::destructItem(m_data, m_size);
        m_alloc.free(m_data);
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::_grow(SizeType n)
{
    auto new_size = m_size + n;
    if (new_size > m_capacity)
    {
        auto new_capacity = m_alloc.getGrow(new_size, m_capacity);
        m_data = m_alloc.resizeContainer(m_data, m_size, m_capacity, new_capacity);
        m_capacity = new_capacity;
    }
}

// ctor & dtor
template<typename T, typename Alloc>
KUN_INLINE Array<T, Alloc>::Array(Alloc alloc)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_alloc(std::move(alloc))
{
}
template<typename T, typename Alloc>
KUN_INLINE Array<T, Alloc>::Array(SizeType size, Alloc alloc)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_alloc(std::move(alloc))
{
    resizeDefault(size);
}
template<typename T, typename Alloc>
KUN_INLINE Array<T, Alloc>::Array(SizeType size, const T& v, Alloc alloc)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_alloc(std::move(alloc))
{
    resize(size, v);
}
template<typename T, typename Alloc>
KUN_INLINE Array<T, Alloc>::Array(const T* p, SizeType n, Alloc alloc)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_alloc(std::move(alloc))
{
    resizeUnsafe(n);
    memory::copyItems(m_data, p, n);
}
template<typename T, typename Alloc>
KUN_INLINE Array<T, Alloc>::Array(std::initializer_list<T> init_list, Alloc alloc)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_alloc(std::move(alloc))
{
    resizeUnsafe(init_list.size());
    memory::copyItems(m_data, init_list.begin(), init_list.size());
}
template<typename T, typename Alloc> KUN_INLINE Array<T, Alloc>::~Array() { release(); }

// copy & move
template<typename T, typename Alloc>
KUN_INLINE Array<T, Alloc>::Array(const Array& other, Alloc alloc)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_alloc(std::move(alloc))
{
    resizeUnsafe(other.size());
    memory::copyItems(m_data, other.data(), other.size());
}
template<typename T, typename Alloc>
KUN_INLINE Array<T, Alloc>::Array(Array&& other)
    : m_data(other.m_data)
    , m_size(other.m_size)
    , m_capacity(other.m_capacity)
    , m_alloc(std::move(other.m_alloc))
{
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

// assign & move assign
template<typename T, typename Alloc> KUN_INLINE Array<T, Alloc>& Array<T, Alloc>::operator=(const Array& rhs)
{
    if (this != &rhs)
    {
        // release and resize
        release(rhs.m_size);
        resizeUnsafe(rhs.m_size);

        // copy items
        memory::copyItems(m_data, rhs.m_data, rhs.m_size);
    }
}
template<typename T, typename Alloc> KUN_INLINE Array<T, Alloc>& Array<T, Alloc>::operator=(Array&& rhs)
{
    if (this != &rhs)
    {
        // release
        release();

        // copy data
        m_data = rhs.m_data;
        m_size = rhs.m_size;
        m_capacity = rhs.m_capacity;
        m_alloc == std::move(rhs.m_alloc);

        // invalidate rhs
        rhs.m_data = nullptr;
        rhs.m_size = 0;
        rhs.capacity = 0;
    }
}

// compare
template<typename T, typename Alloc> KUN_INLINE bool Array<T, Alloc>::operator==(const Array& rhs) const
{
    return m_size == rhs.m_size && memory::compareItems(m_data, rhs.m_data, m_size);
}
template<typename T, typename Alloc> KUN_INLINE bool Array<T, Alloc>::operator!=(const Array& rhs) const { return !(*this == rhs); }

// getter
template<typename T, typename Alloc> KUN_INLINE T*                                 Array<T, Alloc>::data() { return m_data; }
template<typename T, typename Alloc> KUN_INLINE const T*                           Array<T, Alloc>::data() const { return m_data; }
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::size() const { return m_size; }
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::capacity() const { return m_capacity; }
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::slack() const { return m_capacity - m_size; }
template<typename T, typename Alloc> KUN_INLINE Alloc&                             Array<T, Alloc>::allocator() { return m_alloc; }
template<typename T, typename Alloc> KUN_INLINE const Alloc&                       Array<T, Alloc>::allocator() const { return m_alloc; }
template<typename T, typename Alloc> KUN_INLINE bool                               Array<T, Alloc>::empty() { return m_size == 0; }

// validate
template<typename T, typename Alloc> KUN_INLINE bool Array<T, Alloc>::isValidIndex(SizeType idx) const { return idx > 0 && idx < m_size; }
template<typename T, typename Alloc> KUN_INLINE bool Array<T, Alloc>::isValidPointer(const T* p) const { return p >= begin() && p < end(); }

// memory op
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::clear()
{
    if (m_size)
    {
        memory::destructItem(m_data, m_size);
        m_size = 0;
    }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::release(SizeType capacity)
{
    clear();
    _resizeMemory(capacity);
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::reserve(SizeType capacity)
{
    if (capacity > m_capacity)
    {
        _resizeMemory(capacity);
    }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::shrink()
{
    auto new_capacity = m_alloc.getShrink(m_size, m_capacity);
    _resizeMemory(new_capacity);
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::resize(SizeType size, const T& new_value)
{
    // do unsafe resize
    auto old_size = m_size;
    resizeUnsafe(size);

    // add or remove
    if (size > old_size)
    {
        add(new_value, size - old_size);
    }
    else if (size < old_size)
    {
        removeAt(size, old_size - size);
    }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::resizeUnsafe(SizeType size)
{
    // do memory resize
    if (m_capacity < size)
    {
        _resizeMemory(size);
    }

    // directly assign
    m_size = size;
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::resizeZeroed(SizeType size)
{
    // do unsafe resize
    auto old_size = m_size;
    resizeUnsafe(size);

    // add or remove
    if (size > old_size)
    {
        addZeroed(size - old_size);
    }
    else if (size < old_size)
    {
        removeAt(size, old_size - size);
    }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::resizeDefault(SizeType size)
{
    // do unsafe resize
    auto old_size = m_size;
    resizeUnsafe(size);

    // add or remove
    if (size > old_size)
    {
        addDefault(size - old_size);
    }
    else if (size < old_size)
    {
        removeAt(size, old_size - size);
    }
}

}// namespace kun