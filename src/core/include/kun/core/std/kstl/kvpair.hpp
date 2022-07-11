#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"

// KVPair def
namespace kun
{
template<typename Key, typename Value> struct KVPair
{
    // ctor & dtor
    KVPair();
    template<typename TK> KVPair(TK&& k);
    template<typename TK, typename TV> KVPair(TK&& k, TV&& v);
    ~KVPair();

    // copy & move
    KVPair(const KVPair&);
    KVPair(KVPair&&);

    // assign & move assign
    KVPair& operator=(const KVPair&);
    KVPair& operator=(KVPair&&);

    // compare
    bool operator==(const KVPair& rhs) const;
    bool operator!=(const KVPair& rhs) const;
    bool operator<(const KVPair& rhs) const;
    bool operator<=(const KVPair& rhs) const;
    bool operator>(const KVPair& rhs) const;
    bool operator>=(const KVPair& rhs) const;

    Key   key;
    Value value;
};
}// namespace kun

// KVPair impl
namespace kun
{
// ctor & dtor
template<typename Key, typename Value> KUN_INLINE KVPair<Key, Value>::KVPair() = default;
template<typename Key, typename Value>
template<typename TK>
KUN_INLINE KVPair<Key, Value>::KVPair(TK&& k)
    : key(std::forward<TK>(k))
    , value()
{
}
template<typename Key, typename Value>
template<typename TK, typename TV>
KUN_INLINE KVPair<Key, Value>::KVPair(TK&& k, TV&& v)
    : key(std::forward<TK>(k))
    , value(std::forward<TV>(v))
{
}
template<typename Key, typename Value> KUN_INLINE KVPair<Key, Value>::~KVPair() = default;

// copy & move
template<typename Key, typename Value> KUN_INLINE KVPair<Key, Value>::KVPair(const KVPair&) = default;
template<typename Key, typename Value> KUN_INLINE KVPair<Key, Value>::KVPair(KVPair&&) = default;

// assign & move assign
template<typename Key, typename Value> KUN_INLINE KVPair<Key, Value>& KVPair<Key, Value>::operator=(const KVPair&) = default;
template<typename Key, typename Value> KUN_INLINE KVPair<Key, Value>& KVPair<Key, Value>::operator=(KVPair&&) = default;

// compare
template<typename Key, typename Value> KUN_INLINE bool KVPair<Key, Value>::operator==(const KVPair& rhs) const
{
    return key == rhs.key && value == rhs.value;
}
template<typename Key, typename Value> KUN_INLINE bool KVPair<Key, Value>::operator!=(const KVPair& rhs) const { return !(*this == rhs); }
template<typename Key, typename Value> KUN_INLINE bool KVPair<Key, Value>::operator<(const KVPair& rhs) const
{
    return key == rhs.key ? value < rhs.value : key < rhs.key;
}
template<typename Key, typename Value> KUN_INLINE bool KVPair<Key, Value>::operator<=(const KVPair& rhs) const { return !(*this > rhs); }
template<typename Key, typename Value> KUN_INLINE bool KVPair<Key, Value>::operator>(const KVPair& rhs) const
{
    return key == rhs.key ? value > rhs.value : key > rhs.key;
}
template<typename Key, typename Value> KUN_INLINE bool KVPair<Key, Value>::operator>=(const KVPair& rhs) const { return !(*this < rhs); }
}// namespace kun