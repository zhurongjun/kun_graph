#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/algo/intro_sort.hpp"
#include "kun/core/std/kstl/algo/merge_sort.hpp"
#include "kun/core/std/kstl/algo/remove.hpp"
#include "kun/core/std/kstl/algo/find.hpp"
#include "fwd.hpp"

// Array def
namespace kun
{
// TODO. consider support range
// TODO. consider support span
template<typename T, typename Alloc> class Array
{
public:
    using SizeType = typename Alloc::SizeType;

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
    void resizeDefault(SizeType size);
    void resizeZeroed(SizeType size);

    // add
    SizeType add(const T& v, SizeType n = 1);
    SizeType addUnique(const T& v);
    SizeType addUnsafe(SizeType n = 1);
    SizeType addDefault(SizeType n = 1);
    SizeType addZeroed(SizeType n = 1);

    // add at
    void addAt(SizeType idx, const T& v, SizeType n = 1);
    void addAtUnsafe(SizeType idx, SizeType n = 1);
    void addAtDefault(SizeType idx, SizeType n = 1);
    void addAtZeroed(SizeType idx, SizeType n = 1);

    // emplace
    template<typename... Args> SizeType emplace(Args&&... args);
    template<typename... Args> void     emplaceAt(SizeType index, Args&&... args);

    // append
    SizeType append(const Array& arr);
    SizeType append(std::initializer_list<T> init_list);
    SizeType append(T* p, SizeType n);

    // append at
    void appendAt(SizeType idx, const Array& arr);
    void appendAt(SizeType idx, std::initializer_list<T> init_list);
    void appendAt(SizeType idx, T* p, SizeType n);

    // assign
    void assign(const T* p, SizeType n);
    void assign(std::initializer_list<T> init_list);

    // remove
    void                           removeAt(SizeType index, SizeType n = 1);
    void                           removeAtSwap(SizeType index, SizeType n = 1);
    template<typename TK> SizeType remove(const TK& v);
    template<typename TK> SizeType removeSwap(const TK& v);
    template<typename TK> SizeType removeLast(const TK& v);
    template<typename TK> SizeType removeLastSwap(const TK& v);
    template<typename TK> SizeType removeAll(const TK& v);
    template<typename TK> SizeType removeAllSwap(const TK& v);

    // remove if
    template<typename TP> SizeType removeIf(TP&& p);
    template<typename TP> SizeType removeIfSwap(TP&& p);
    template<typename TP> SizeType removeLastIf(TP&& p);
    template<typename TP> SizeType removeLastIfSwap(TP&& p);
    template<typename TP> SizeType removeAllIf(TP&& p);
    template<typename TP> SizeType removeAllIfSwap(TP&& p);

    // modify
    T&       operator[](SizeType index);
    const T& operator[](SizeType index) const;

    // find
    template<typename TK> T*       find(const TK& v);
    template<typename TK> T*       findLast(const TK& v);
    template<typename TK> const T* find(const TK& v) const;
    template<typename TK> const T* findLast(const TK& v) const;

    // find if
    template<typename TP> T*       findIf(TP&& p);
    template<typename TP> T*       findLastIf(TP&& p);
    template<typename TP> const T* findIf(TP&& p) const;
    template<typename TP> const T* findLastIf(TP&& p) const;

    // contain
    bool contain(const T& v) const;

    // contain if
    template<typename TP> bool containIf(TP&& p) const;

    // sort
    template<typename TP = Less<T>> void sort(TP&& p = TP());
    template<typename TP = Less<T>> void sortStable(TP&& p = TP());

    // support heap
    T&                                       heapTop();
    template<typename TP = Less<T>> void     heapify(TP&& p = TP());
    template<typename TP = Less<T>> bool     isHeap(TP&& p = TP());
    template<typename TP = Less<T>> SizeType heapPush(T&& v, TP&& p = TP());
    template<typename TP = Less<T>> SizeType heapPush(const T& v, TP&& p = TP());
    template<typename TP = Less<T>> void     heapPop(TP&& p = TP());
    template<typename TP = Less<T>> T        heapPopGet(TP&& p = TP());
    template<typename TP = Less<T>> void     heapRemoveAt(SizeType index, TP&& p = TP());
    template<typename TP = Less<T>> void     heapSort(TP&& p = TP());

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

    // grow memory
    if (new_size > m_capacity)
    {
        auto new_capacity = m_alloc.getGrow(new_size, m_capacity);
        m_data = m_alloc.resizeContainer(m_data, m_size, m_capacity, new_capacity);
        m_capacity = new_capacity;
    }

    // update size
    m_size = new_size;
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
        // clear and resize
        clear();
        resizeUnsafe(rhs.m_size);

        // copy items
        memory::copyItems(m_data, rhs.m_data, rhs.m_size);
    }
    return *this;
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
        m_alloc = std::move(rhs.m_alloc);

        // invalidate rhs
        rhs.m_data = nullptr;
        rhs.m_size = 0;
        rhs.m_capacity = 0;
    }
    return *this;
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
template<typename T, typename Alloc> KUN_INLINE bool Array<T, Alloc>::isValidIndex(SizeType idx) const { return idx >= 0 && idx < m_size; }
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
    // explicit size
    if (size > m_capacity)
    {
        _resizeMemory(size);
    }

    // add or remove
    if (size > m_size)
    {
        add(new_value, size - m_size);
    }
    else if (size < m_size)
    {
        removeAt(size, m_size - size);
    }

    // set size
    m_size = size;
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::resizeUnsafe(SizeType size)
{
    // explicit size
    if (size > m_capacity)
    {
        _resizeMemory(size);
    }

    // add or remove
    if (size > m_size)
    {
        addUnsafe(size - m_size);
    }
    else if (size < m_size)
    {
        removeAt(size, m_size - size);
    }

    // set size
    m_size = size;
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::resizeDefault(SizeType size)
{
    // explicit size
    if (size > m_capacity)
    {
        _resizeMemory(size);
    }

    // add or remove
    if (size > m_size)
    {
        addDefault(size - m_size);
    }
    else if (size < m_size)
    {
        removeAt(size, m_size - size);
    }

    // set size
    m_size = size;
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::resizeZeroed(SizeType size)
{
    // explicit size
    if (size > m_capacity)
    {
        _resizeMemory(size);
    }

    // add or remove
    if (size > m_size)
    {
        addZeroed(size - m_size);
    }
    else if (size < m_size)
    {
        removeAt(size, m_size - size);
    }

    // set size
    m_size = size;
}

// add
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::add(const T& v, SizeType n)
{
    auto old_size = addUnsafe(n);
    for (SizeType i = old_size; i < m_size; ++i) { new (m_data + i) T(v); }
    return old_size;
}
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::addUnique(const T& v)
{
    if (auto p = find(v))
    {
        return p - m_data;
    }
    else
    {
        return add(v);
    }
}
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::addUnsafe(SizeType n)
{
    auto old_size = m_size;
    _grow(n);
    return old_size;
}
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::addDefault(SizeType n)
{
    auto old_size = addUnsafe(n);
    for (SizeType i = old_size; i < m_size; ++i) { new (m_data + i) T(); }
    return old_size;
}
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::addZeroed(SizeType n)
{
    auto old_size = addUnsafe(n);
    memory::memzero(m_data + old_size, n * sizeof(T));
    return old_size;
}

// add at
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::addAt(SizeType idx, const T& v, SizeType n)
{
    KUN_Assert(isValidIndex(idx));
    addAtUnsafe(idx, n);
    for (SizeType i = 0; i < n; ++i) { new (m_data + idx + i) T(v); }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::addAtUnsafe(SizeType idx, SizeType n)
{
    KUN_Assert(isValidIndex(idx));
    auto move_n = m_size - idx;
    _grow(n);
    memory::moveItems(m_data + idx + n, m_data + idx, move_n);
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::addAtDefault(SizeType idx, SizeType n)
{
    KUN_Assert(isValidIndex(idx));
    addAtUnsafe(idx, n);
    for (SizeType i = 0; i < n; ++i) { new (m_data + idx + i) T(); }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::addAtZeroed(SizeType idx, SizeType n)
{
    KUN_Assert(isValidIndex(idx));
    addAtUnsafe(idx, n);
    memory::memzero(m_data + idx, n * sizeof(T));
}

// emplace
template<typename T, typename Alloc> template<typename... Args> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::emplace(Args&&... args)
{
    auto old_index = addUnsafe();
    new (m_data + old_index) T(std::forward<Args>(args)...);
    return old_index;
}
template<typename T, typename Alloc> template<typename... Args> KUN_INLINE void Array<T, Alloc>::emplaceAt(SizeType index, Args&&... args)
{
    addAtUnsafe(index);
    new (m_data + index) T(std::forward<Args>(args)...);
}

// append
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::append(const Array& arr)
{
    if (arr.m_size)
    {
        auto old_index = addUnsafe(arr.size());
        memory::copyItems(m_data + old_index, arr.m_data, arr.m_size);
        return old_index;
    }
    return m_size;
}
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::append(std::initializer_list<T> init_list)
{
    if (init_list.size())
    {
        auto old_index = addUnsafe(init_list.size());
        memory::copyItems(m_data + old_index, init_list.begin(), init_list.size());
        return old_index;
    }
    return m_size;
}
template<typename T, typename Alloc> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::append(T* p, SizeType n)
{
    if (n)
    {
        auto old_index = addUnsafe(n);
        memory::copyItems(m_data + old_index, p, n);
        return old_index;
    }
    return m_size;
}

// append at
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::appendAt(SizeType idx, const Array& arr)
{
    KUN_Assert(isValidIndex(idx));
    if (arr.m_size)
    {
        addAtUnsafe(idx, arr.m_size);
        memory::copyItems(m_data + idx, arr.m_data, arr.m_size);
    }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::appendAt(SizeType idx, std::initializer_list<T> init_list)
{
    KUN_Assert(isValidIndex(idx));
    if (init_list.size())
    {
        addAtUnsafe(idx, init_list.size());
        memory::copyItems(m_data + idx, init_list.begin(), init_list.size());
    }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::appendAt(SizeType idx, T* p, SizeType n)
{
    KUN_Assert(isValidIndex(idx));
    if (n)
    {
        addAtUnsafe(idx, n);
        memory::copyItems(m_data + idx, p, n);
    }
}

// assign
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::assign(const T* p, SizeType n)
{
    // clear and resize
    clear();
    resizeUnsafe(n);

    // copy items
    memory::copyItems(m_data, p, n);
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::assign(std::initializer_list<T> init_list)
{
    assign(init_list.begin(), init_list.size());
}

// remove
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::removeAt(SizeType index, SizeType n)
{
    KUN_Assert(index >= 0 && index + n <= m_size);

    if (n)
    {
        // calc move size
        auto move_n = m_size - index - n;

        // move data
        if (move_n)
        {
            memory::moveItems(m_data + index, m_data + m_size - move_n, move_n);
        }

        // destruct data
        memory::destructItem(m_data + index + move_n, n);

        // update size
        m_size -= n;
    }
}
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::removeAtSwap(SizeType index, SizeType n)
{
    KUN_Assert(index >= 0 && index + n <= m_size);
    if (n)
    {
        // calc move size
        auto move_n = std::min(m_size - index - n, n);

        // move data
        if (move_n)
        {
            memory::moveItems(m_data + index, m_data + m_size - move_n, move_n);
        }

        // destruct data
        memory::destructItem(m_data + index + move_n, n);

        // update size
        m_size -= n;
    }
}
template<typename T, typename Alloc> template<typename TK> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::remove(const TK& v)
{
    if (auto p = find(v))
    {
        auto pos = p - m_data;
        removeAt(pos);
        return pos;
    }
    return npos;
}
template<typename T, typename Alloc> template<typename TK> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeSwap(const TK& v)
{
    if (auto p = find(v))
    {
        auto pos = p - m_data;
        removeAtSwap(pos);
        return pos;
    }
    return npos;
}
template<typename T, typename Alloc> template<typename TK> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeLast(const TK& v)
{
    if (auto p = findLast(v))
    {
        auto pos = p - m_data;
        removeAt(pos);
        return pos;
    }
    return npos;
}
template<typename T, typename Alloc> template<typename TK> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeLastSwap(const TK& v)
{
    if (auto p = findLast(v))
    {
        auto pos = p - m_data;
        removeAtSwap(pos);
        return pos;
    }
    return npos;
}
template<typename T, typename Alloc> template<typename TK> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeAll(const TK& v)
{
    return removeAllIf([&v](const T& a) { return a == v; });
}
template<typename T, typename Alloc> template<typename TK> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeAllSwap(const TK& v)
{
    return removeAllIfSwap([&v](const T& a) { return a == v; });
}

// remove by
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeIf(TP&& p)
{
    if (auto find_p = findIf(std::forward<TP>(p)))
    {
        auto pos = find_p - m_data;
        removeAt(pos);
        return pos;
    }
    return npos;
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeIfSwap(TP&& p)
{
    if (auto find_p = findIf(std::forward<TP>(p)))
    {
        auto pos = find_p - m_data;
        removeAtSwap(pos);
        return pos;
    }
    return npos;
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeLastIf(TP&& p)
{
    if (auto find_p = findLastIf(std::forward<TP>(p)))
    {
        auto pos = find_p - m_data;
        removeAt(pos);
        return pos;
    }
    return npos;
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeLastIfSwap(TP&& p)
{
    if (auto find_p = findLastIf(std::forward<TP>(p)))
    {
        auto pos = find_p - m_data;
        removeAtSwap(pos);
        return pos;
    }
    return npos;
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeAllIf(TP&& p)
{
    T*       pos = algo::removeIf(begin(), end(), std::forward<TP>(p));
    SizeType n = end() - pos;
    if (n)
    {
        memory::destructItem(pos, n);
    }
    m_size -= n;
    return n;
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::removeAllIfSwap(TP&& p)
{
    T*       pos = algo::removeIfSwap(begin(), end(), p);
    SizeType n = end() - pos;
    if (n)
    {
        memory::destructItem(pos, n);
    }
    m_size -= n;
    return n;
}

// modify
template<typename T, typename Alloc> KUN_INLINE T& Array<T, Alloc>::operator[](SizeType index)
{
    KUN_Assert(isValidIndex(index));
    return *(m_data + index);
}
template<typename T, typename Alloc> KUN_INLINE const T& Array<T, Alloc>::operator[](SizeType index) const
{
    KUN_Assert(isValidIndex(index));
    return *(m_data + index);
}

// find
template<typename T, typename Alloc> template<typename TK> KUN_INLINE T* Array<T, Alloc>::find(const TK& v)
{
    return findIf([&v](const T& a) { return a == v; });
}
template<typename T, typename Alloc> template<typename TK> KUN_INLINE T* Array<T, Alloc>::findLast(const TK& v)
{
    return findLastIf([&v](const T& a) { return a == v; });
}
template<typename T, typename Alloc> template<typename TK> KUN_INLINE const T* Array<T, Alloc>::find(const TK& v) const
{
    return findIf([&v](const T& a) { return a == v; });
}
template<typename T, typename Alloc> template<typename TK> KUN_INLINE const T* Array<T, Alloc>::findLast(const TK& v) const
{
    return findLastIf([&v](const T& a) { return a == v; });
}

// find by
template<typename T, typename Alloc> template<typename TP> KUN_INLINE T* Array<T, Alloc>::findIf(TP&& p)
{
    return algo::find(begin(), end(), std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE T* Array<T, Alloc>::findLastIf(TP&& p)
{
    return algo::findLast(begin(), end(), std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE const T* Array<T, Alloc>::findIf(TP&& p) const
{
    return algo::find(begin(), end(), std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE const T* Array<T, Alloc>::findLastIf(TP&& p) const
{
    return algo::findLast(begin(), end(), std::forward<TP>(p));
}

// contain
template<typename T, typename Alloc> KUN_INLINE bool Array<T, Alloc>::contain(const T& v) const { return find(v); }

// contain if
template<typename T, typename Alloc> template<typename TP> KUN_INLINE bool Array<T, Alloc>::containIf(TP&& p) const
{
    return findIf(std::forward<TP>(p));
}

// sort
template<typename T, typename Alloc> template<typename TP> KUN_INLINE void Array<T, Alloc>::sort(TP&& p)
{
    algo::introSort(begin(), end(), std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE void Array<T, Alloc>::sortStable(TP&& p)
{
    algo::mergeSort(begin(), end(), std::forward<TP>(p));
}

// support heap
template<typename T, typename Alloc> KUN_INLINE T&                         Array<T, Alloc>::heapTop() { return *m_data; }
template<typename T, typename Alloc> template<typename TP> KUN_INLINE void Array<T, Alloc>::heapify(TP&& p)
{
    algo::heapify(m_data, m_size, std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE bool Array<T, Alloc>::isHeap(TP&& p)
{
    return algo::isHeap(m_data, m_size, std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::heapPush(T&& v, TP&& p)
{
    auto idx = emplace(std::move(v));
    return algo::heapSiftUp(m_data, (SizeType)0, idx, std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE typename Array<T, Alloc>::SizeType Array<T, Alloc>::heapPush(const T& v, TP&& p)
{
    auto idx = add(v);
    return algo::heapSiftUp(m_data, (SizeType)0, idx, std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE void Array<T, Alloc>::heapPop(TP&& p)
{
    removeAtSwap(0);
    algo::heapSiftDown(m_data, (SizeType)0, m_size, std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE T Array<T, Alloc>::heapPopGet(TP&& p)
{
    T result = std::move(*m_data);
    heapPop(std::forward<TP>(p));
    return result;
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE void Array<T, Alloc>::heapRemoveAt(SizeType index, TP&& p)
{
    removeAtSwap(index);

    algo::heapSiftDown(m_data, index, m_size, std::forward<TP>(p));
    algo::heapSiftUp(m_data, (SizeType)0, std::min(index, m_size - 1), std::forward<TP>(p));
}
template<typename T, typename Alloc> template<typename TP> KUN_INLINE void Array<T, Alloc>::heapSort(TP&& p)
{
    algo::heapSort(m_data, m_size, std::forward<TP>(p));
}

// support stack
template<typename T, typename Alloc> KUN_INLINE void Array<T, Alloc>::pop() { removeAt(m_size - 1, 1); }
template<typename T, typename Alloc> KUN_INLINE T    Array<T, Alloc>::popGet()
{
    T result = std::move(*(m_data + m_size - 1));
    pop();
    return result;
}
template<typename T, typename Alloc> KUN_INLINE void     Array<T, Alloc>::push(const T& v) { add(v); }
template<typename T, typename Alloc> KUN_INLINE void     Array<T, Alloc>::push(T&& v) { emplace(std::move(v)); }
template<typename T, typename Alloc> KUN_INLINE T&       Array<T, Alloc>::top() { return *(m_data + m_size - 1); }
template<typename T, typename Alloc> KUN_INLINE const T& Array<T, Alloc>::top() const { return *(m_data + m_size - 1); }
template<typename T, typename Alloc> KUN_INLINE T&       Array<T, Alloc>::bottom() { return *m_data; }
template<typename T, typename Alloc> KUN_INLINE const T& Array<T, Alloc>::bottom() const { return *m_data; }

// support foreach
template<typename T, typename Alloc> KUN_INLINE T*       Array<T, Alloc>::begin() { return m_data; }
template<typename T, typename Alloc> KUN_INLINE T*       Array<T, Alloc>::end() { return m_data + m_size; }
template<typename T, typename Alloc> KUN_INLINE const T* Array<T, Alloc>::begin() const { return m_data; }
template<typename T, typename Alloc> KUN_INLINE const T* Array<T, Alloc>::end() const { return m_data + m_size; }
}// namespace kun