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
    Array(SizeType size, Alloc alloc = Alloc);
    Array(SizeType size, const T& v, Alloc alloc = Alloc());
    Array(T* p, SizeType n, Alloc alloc = Alloc());
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
    bool isValidIndex(SizeType idx);
    bool isValidPointer(const T* p);

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
    SizeType remove(const T& v);
    SizeType removeSwap(const T& v);
    SizeType removeLast(const T& v);
    SizeType removeLastSwap(const T& v);
    SizeType removeAll(const T& v);
    SizeType removeAllSwap(const T& v);

    // remove by
    template<typename TP> SizeType removeBy(TP&& p);
    template<typename TP> SizeType removeBySwap(TP&& p);

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
    void _shink();
    void _resize(SizeType size);
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

}