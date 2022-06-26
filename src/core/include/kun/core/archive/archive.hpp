#pragma once
#include "kun/core/config.h"
#include "kun/core/std/stl.hpp"
#include "kun/core/functional/raii.hpp"
#include "archive_state.hpp"
#include "archive_string.hpp"
#include "named_value.hpp"

// archive
namespace kun
{
class Archive : public ArchiveState
{
    friend class ArchiveArray;
    friend class ArchiveMap;
    friend class ArchiveStructure;

public:
    enum class EValueType
    {
        Int,
        Uint,
        Float,
        Bool,
        Binary
    };

public:
    KUN_INLINE Archive() = default;
    virtual ~Archive() = default;

    // base type serialize function
    virtual void serialize(StringView name, void* value, u64 length, EValueType type = EValueType::Binary) = 0;

    // string serialize
    virtual void serialize(StringView name, ArchiveString value) = 0;

    // hold string(for NamedValue::name)
    // usually, NamedValue::name only used in serialize() function
    // but, for some Archive, we need keep NamedValue::name valid before Archive clear or destroyed
    // if you can't promise it, use holdString to keep NamedValue::name valid
    virtual StringView holdString(StringView raw_string) { return raw_string; }

protected:
    // scope function, use RAII
    // !!! DON'T CALL DIRECTLY
    virtual void beginArray(StringView name, Size& size) = 0;
    virtual void endArray(StringView name) = 0;
    virtual void beginMap(StringView name, Size& size) = 0;
    virtual void endMap(StringView name) = 0;
    virtual void beginStructure(StringView name) = 0;
    virtual void endStructure(StringView name) = 0;
};
}// namespace kun

// RAII tools
namespace kun
{
class ArchiveArray
{
public:
    KUN_INLINE ArchiveArray(Archive& ar, StringView name, Size& size) noexcept
        : m_archive(ar)
        , m_name(name)
        , m_size(size)
    {
    }

    KUN_INLINE void lock() { m_archive.beginArray(m_name, m_size); }
    KUN_INLINE void unlock() { m_archive.endArray(m_name); }

private:
    Archive& m_archive;
    StringView m_name;
    Size& m_size;
};
class ArchiveMap
{
public:
    KUN_INLINE ArchiveMap(Archive& ar, StringView name, Size& size) noexcept
        : m_archive(ar)
        , m_name(name)
        , m_size(size)
    {
    }

    KUN_INLINE void lock() { m_archive.beginMap(m_name, m_size); }
    KUN_INLINE void unlock() { m_archive.endMap(m_name); }

private:
    Archive& m_archive;
    StringView m_name;
    Size& m_size;
};
class ArchiveStructure
{
public:
    KUN_INLINE explicit ArchiveStructure(Archive& ar, StringView name = "") noexcept
        : m_archive(ar)
        , m_name(name)
    {
    }

    KUN_INLINE void lock() { m_archive.beginStructure(m_name); }
    KUN_INLINE void unlock() { m_archive.endStructure(m_name); }

private:
    Archive& m_archive;
    StringView m_name;
};
}// namespace kun

// pattern match
namespace kun
{
// traits
namespace detail
{
template<typename T> using has_serialzie = decltype(serialize(std::declval<T&>(), std::declval<Archive&>()));
template<typename T> using has_serialize_member = decltype(std::declval<T&>().serialize(std::declval<Archive&>()));
template<typename T> using has_adapter_serialize_member = decltype(std::declval<T&>().adapter_serialize(std::declval<Archive&>()));
}// namespace detail
template<typename T> inline constexpr bool has_serialize_v = is_detected_v<detail::has_serialzie, T>;
template<typename T> inline constexpr bool has_serialize_member_v = is_detected_v<detail::has_serialize_member, T>;
template<typename T> inline constexpr bool has_adapter_serialize_member_v = is_detected_v<detail::has_adapter_serialize_member, T>;

// serialize for custom type
template<typename T> KUN_INLINE std::enable_if_t<has_serialize_member_v<T>> serialize(T& value, Archive& ar) { value.serialize(ar); }

// serialize for any struct/class or custom type
template<typename T> KUN_INLINE std::enable_if_t<has_serialize_v<T>, Archive&> operator&(Archive& ar, NamedValue<T> v)
{
    RAII raii_scope(ArchiveStructure(ar, v.name));
    ::kun::serialize(v.value, ar);
    return ar;
}

// serialize for adapter
template<typename T> KUN_INLINE std::enable_if_t<has_adapter_serialize_member_v<T>, Archive&> operator&(Archive& ar, NamedValue<T> v)
{
    v.value.adapter_serialize(ar, v.name);
    return ar;
}

// serialize for enum
template<typename T> KUN_INLINE std::enable_if_t<std::is_enum_v<T>, Archive&> operator&(Archive& ar, NamedValue<T> v)
{
    using underlying_type = std::underlying_type_t<T>;
    ar& NamedValue<underlying_type>(v.name, (*reinterpret_cast<underlying_type*>(&v.value)));
    return ar;
}
}// namespace kun