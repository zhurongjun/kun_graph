#pragma once
#include "kun/core/config.h"
#include "kun/core/std/stl.hpp"
#include "archive.hpp"

// base type
namespace kun
{
#define KUN_BASE_TYPE_ARCHIVE_OP(type, type_enum)                                                                                                    \
    KUN_INLINE Archive& operator&(Archive& ar, NamedValue<type> v)                                                                                   \
    {                                                                                                                                                \
        ar.serialize(v.name, &v.value, sizeof(v.value), Archive::EValueType::type_enum);                                                             \
        return ar;                                                                                                                                   \
    }

KUN_BASE_TYPE_ARCHIVE_OP(i8, Int)
KUN_BASE_TYPE_ARCHIVE_OP(i16, Int)
KUN_BASE_TYPE_ARCHIVE_OP(i32, Int)
KUN_BASE_TYPE_ARCHIVE_OP(i64, Int)

KUN_BASE_TYPE_ARCHIVE_OP(u8, Uint)
KUN_BASE_TYPE_ARCHIVE_OP(u16, Uint)
KUN_BASE_TYPE_ARCHIVE_OP(u32, Uint)
KUN_BASE_TYPE_ARCHIVE_OP(u64, Uint)

KUN_BASE_TYPE_ARCHIVE_OP(f32, Float)
KUN_BASE_TYPE_ARCHIVE_OP(f64, Float)

KUN_BASE_TYPE_ARCHIVE_OP(bool, Bool)

#undef KUN_BASE_TYPE_ARCHIVE_OP
}// namespace kun

// containers
// we provide serializer wrapper instead
namespace kun
{
// for array
template<typename T, typename ElemPolicy> struct SerArray
{
    static void defaultPolicy(Archive& ar, T& v) { ar& NamedValue<T>(v); }

    KUN_INLINE SerArray(Array<T>& arr, ElemPolicy& policy = &defaultPolicy)
        : m_arr(arr)
        , m_policy(std::forward<ElemPolicy>(policy))
    {
    }

    KUN_INLINE void adapter_serialize(Archive& ar, StringView name)
    {
        if (ar.is_loading)
        {
            Size size = 0;
            RAII raii(ArchiveArray(ar, name, size));
            if (size != 0)
            {
                m_arr.resize(size);
                for (u32 i = 0; i < size; ++i) { m_policy(ar, m_arr[i]); }
            }
        }
        else
        {
            Size size = m_arr.size();
            RAII raii(ArchiveArray(ar, name, size));
            for (i32 i = 0; i < size; ++i) { m_policy(ar, m_arr[i]); }
        }
    }

private:
    Array<T>& m_arr;
    ElemPolicy& m_policy;
};

// for uset
template<typename T, typename ElemPolicy> struct SerUSet
{
    static void defaultPolicy(Archive& ar, T& v) { ar& NamedValue<T>(v); }

    KUN_INLINE SerUSet(USet<T>& set, ElemPolicy& policy = &defaultPolicy)
        : m_set(set)
        , m_policy(std::forward<ElemPolicy>(policy))
    {
    }

    KUN_INLINE void adapter_serialize(Archive& ar, StringView name)
    {
        if (ar.is_loading)
        {
            Size size = 0;
            RAII raii(ArchiveArray(ar, name, size));
            if (size != 0)
            {
                m_set.clear();
                m_set.reserve(size);
                for (u32 i = 0; i < size; ++i)
                {
                    T item;
                    m_policy(ar, item);
                    m_set.insert(std::move(item));
                }
            }
        }
        else
        {
            Size size = m_set.size();
            RAII raii(ArchiveArray(ar, name, size));
            for (auto& item : m_set) { m_policy(ar, item); }
        }
    }

private:
    USet<T>& m_set;
    ElemPolicy& m_policy;
};

// for umap
template<typename K, typename V, typename KPolicy, typename VPolicy> struct SerUMap
{
    static void defaultKPolicy(Archive& ar, K& v) { ar& NamedValue<K>("k", v); }
    static void defaultVPolicy(Archive& ar, V& v) { ar& NamedValue<V>("v", v); }

    KUN_INLINE SerUMap(UMap<K, V>& map, KPolicy& k_policy = &defaultKPolicy, VPolicy& v_policy = &defaultVPolicy)
        : m_map(map)
        , m_k_policy(std::forward<KPolicy>(k_policy))
        , m_v_policy(std::forward<VPolicy>(v_policy))
    {
    }

    KUN_INLINE void adapter_serialize(Archive& ar, StringView name)
    {
        if (ar.is_loading)
        {
            Size size = 0;
            RAII raii(ArchiveArray(ar, name, size));
            if (size != 0)
            {
                m_map.clear();
                m_map.reserve(size);
                for (u32 i = 0; i < size; ++i)
                {
                    RAII item_raii((ArchiveStructure(ar)));
                    K k;
                    V v;
                    m_k_policy(ar, k);
                    m_v_policy(ar, v);
                    m_map.insert_or_assign(std::move(k), std::move(v));
                }
            }
        }
        else
        {
            Size size = m_map.size();
            RAII raii(ArchiveArray(ar, name, size));
            for (auto& item : m_map)
            {
                RAII item_raii((ArchiveStructure(ar)));
                m_k_policy(ar, item.first);
                m_v_policy(ar, item.second);
            }
        }
    }

private:
    UMap<K, V>& m_map;
    KPolicy& m_k_policy;
    VPolicy& m_v_policy;
};

}// namespace kun

// string
namespace kun
{
KUN_INLINE Archive& operator&(Archive& ar, NamedValue<String> str)
{
    auto get_str = [](void* data) -> const char* { return static_cast<String*>(data)->c_str(); };
    auto set_str = [](void* data, const char* str, Size len) { (*static_cast<String*>(data)) = String(str, len); };
    auto get_len = [](void* data) -> Size { return static_cast<String*>(data)->length(); };

    ar.serialize(str.name, ArchiveString(&str.value, get_str, set_str, get_len));
    return ar;
}

KUN_INLINE Archive& operator&(Archive& ar, NamedValue<StringView> str)
{
    auto get_str = [](void* data) -> const char* { return static_cast<StringView*>(data)->data(); };
    auto set_str = [](void* data, const char* str, Size len) { throw StrException("can't write string view when serialize"); };
    auto get_len = [](void* data) -> Size { return static_cast<StringView*>(data)->length(); };

    ar.serialize(str.name, ArchiveString(&str.value, get_str, set_str, get_len));
    return ar;
}

KUN_INLINE Archive& operator&(Archive& ar, NamedValue<Name> str)
{
    auto get_str = [](void* data) -> const char* { return static_cast<Name*>(data)->str().data(); };
    auto set_str = [](void* data, const char* str, Size len) { (*static_cast<Name*>(data)) = Name(StringView(str, len)); };
    auto get_len = [](void* data) -> Size { return static_cast<Name*>(data)->length(); };

    ar.serialize(str.name, ArchiveString(&str.value, get_str, set_str, get_len));
    return ar;
}
}// namespace kun