#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/basic.hpp"
#include "functor.hpp"

namespace kun::algo
{
// heap jump tools
template<typename T> KUN_INLINE constexpr T    heapLChildIdx(T index) { return index * 2 + 1; }
template<typename T> KUN_INLINE constexpr T    heapRChildIdx(T index) { return heapLChildIdx(index) + 1; }
template<typename T> KUN_INLINE constexpr T    heapParentIdx(T index) { return index ? (index - 1) / 2 : 0; }
template<typename T> KUN_INLINE constexpr bool heapIsLeaf(T index, T count) { return heapLChildIdx(index) >= count; }

// sift down
template<typename T, typename TS, typename TP = Less<>> KUN_INLINE void heapSiftDown(T heap, TS idx, TS count, TP&& p = TP())
{
    while (!heapIsLeaf(idx, count))
    {
        const TS l_child_idx = heapLChildIdx(idx);
        const TS r_child_idx = l_child_idx + 1;

        // find min child
        TS min_child_idx = l_child_idx;
        if (r_child_idx < count)
        {
            min_child_idx = p(*(heap + l_child_idx), *(heap + r_child_idx)) ? l_child_idx : r_child_idx;
        }

        // now element is on his location
        if (!p(*(heap + min_child_idx), *(heap + idx)))
            break;

        ::kun::swap(*(heap + idx), *(heap + min_child_idx));
        idx = min_child_idx;
    }
}

// sift up
template<class T, typename TS, class TP = Less<>> KUN_INLINE TS heapSiftUp(T* heap, TS root_idx, TS node_idx, TP&& p = TP())
{
    while (node_idx > root_idx)
    {
        TS parent_idx = heapParentIdx(node_idx);

        // now element is on his location
        if (!p(*(heap + node_idx), *(heap + parent_idx)))
            break;

        ::kun::swap(*(heap + node_idx), *(heap + parent_idx));
        node_idx = parent_idx;
    }

    return node_idx;
}

// is heap
template<typename T, typename TS, typename TP = Less<>> KUN_INLINE bool isHeap(T* heap, TS count, TP&& p = TP())
{
    for (TS i = 1; i < count; ++i)
    {
        if (p(*(heap + i), *(heap + heapParentIdx(i))))
            return false;
    }
    return true;
}

// heapify
template<typename T, typename TS, typename TP = Less<>> KUN_INLINE void heapify(T* heap, TS count, TP&& p = TP())
{
    if (count > 1)
    {
        TS idx = heapParentIdx(count - 1);
        while (true)
        {
            heapSiftDown(heap, idx, count, std::forward<TP>(p));
            if (idx == 0)
                return;
            --idx;
        }
    }
}

// heap sort
template<typename T, typename TS, class TP = Less<>> KUN_INLINE void heapSort(T heap, TS count, TP&& p = TP())
{
    auto reverse_pred = [&](const auto& a, const auto& b) -> bool { return !p(a, b); };

    // use reverse_pred heapify, and pop head swap to tail
    heapify(heap, count, reverse_pred);

    for (TS cur_count = count - 1; cur_count > 0; --cur_count)
    {
        ::kun::swap(*heap, *(heap + cur_count));
        heapSiftDown(heap, (TS)0, cur_count, reverse_pred);
    }
}
}// namespace kun::algo