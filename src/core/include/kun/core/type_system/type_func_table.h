#pragma once
#include "kun/core/config.h"
#include "kun/core/std/stl.hpp"
#include "kun/core/archive/archive.hpp"

// func table
namespace kun
{
struct TypeFuncTable
{
    // ctor & dtor & init & shutdown
    using LifeCircleFunc = void (*)(void*);
    LifeCircleFunc ctor = nullptr;
    LifeCircleFunc init = nullptr;
    LifeCircleFunc shutdown = nullptr;
    LifeCircleFunc dtor = nullptr;

    // serialize
    using SerializeFunc = void (*)(void*, const char* name, Archive& ar);
    SerializeFunc serialize = nullptr;

    // copy & move
    using CopyFunc = void (*)(void*, const void*);
    using MoveFunc = void (*)(void*, void*);
    CopyFunc copy = nullptr;
    MoveFunc move = nullptr;
    CopyFunc assign = nullptr;
    MoveFunc move_assign = nullptr;

    // swap & hash
    using SwapFunc = void (*)(void*, void*);
    using HashFunc = size_t (*)(const void*);
    SwapFunc swap = nullptr;
    HashFunc hash = nullptr;

    // mathematics
    using MathematicsFunc = void (*)(void*, const void*, const void*);
    MathematicsFunc add = nullptr;
    MathematicsFunc sub = nullptr;
    MathematicsFunc mul = nullptr;
    MathematicsFunc div = nullptr;

    // compare
    using CompareFunc = bool (*)(const void*, const void*);
    CompareFunc equal = nullptr;
    CompareFunc less = nullptr;
    CompareFunc greater = nullptr;
};
}// namespace kun

// default func table maker impl
namespace kun
{
namespace __test
{
template<typename T> using has_init = decltype(std::declval<T>().init());
template<typename T> using has_shutdown = decltype(std::declval<T>().shutdown());
template<typename T> using has_serialize = decltype(std::declval<Archive&>() & NamedValue("", std::declval<T&>()));
}// namespace __test

template<typename T> struct TypeFuncTableMakerDefault
{
    // ctor & dtor & init & shutdown
    constexpr static TypeFuncTable::LifeCircleFunc ctor()
    {
        if constexpr (has_default_ctor_v<T>)
        {
            return [](void* p) { new (p) T(); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::LifeCircleFunc init()
    {
        if constexpr (is_detected_v<__test::has_init, T>)
        {
            return [](void* p) { reinterpret_cast<T*>(p)->init(); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::LifeCircleFunc shutdown()
    {
        if constexpr (is_detected_v<__test::has_shutdown, T>)
        {
            return [](void* p) { reinterpret_cast<T*>(p)->shutdown(); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::LifeCircleFunc dtor()
    {
        return [](void* p) { reinterpret_cast<T*>(p)->~T(); };
    }

    // serialize
    constexpr static TypeFuncTable::SerializeFunc serialize()
    {
        if constexpr (is_detected_v<__test::has_serialize, T>)
        {
            return [](void* p, const char* name, Archive& ar) { ar& NamedValue(name, *reinterpret_cast<T*>(p)); };
        }
        else
        {
            return nullptr;
        }
    }

    // copy & move
    constexpr static TypeFuncTable::CopyFunc copy()
    {
        if constexpr (has_copy_ctor_v<T>)
        {
            return [](void* p, const void* other) { new (p) T(*reinterpret_cast<const T*>(other)); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::MoveFunc move()
    {
        if constexpr (has_move_ctor_v<T>)
        {
            return [](void* p, void* other) { new (p) T(std::move(*reinterpret_cast<T*>(other))); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::CopyFunc assign()
    {
        if constexpr (has_assign_v<T>)
        {
            return [](void* p, const void* other) { *reinterpret_cast<T*>(p) = *reinterpret_cast<const T*>(other); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::MoveFunc move_assign()
    {
        if constexpr (has_move_assign_v<T>)
        {
            return [](void* p, void* other) { *reinterpret_cast<T*>(p) = std::move(*reinterpret_cast<T*>(other)); };
        }
        else
        {
            return nullptr;
        }
    }

    // swap & hash
    constexpr static TypeFuncTable::SwapFunc swap()
    {
        if constexpr (is_swapable_v<T>)
        {
            return [](void* a, void* b) { std::swap(*reinterpret_cast<T*>(a), *reinterpret_cast<T*>(b)); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::HashFunc hash()
    {
        if constexpr (is_hashable_v<T>)
        {
            return [](const void* p) { return Hash<T>()(*reinterpret_cast<const T*>(p)); };
        }
        else
        {
            return nullptr;
        }
    }

    // mathematics
    constexpr static TypeFuncTable::MathematicsFunc add()
    {
        if constexpr (has_add_v<T>)
        {
            return [](void* out, const void* lhs, const void* rhs)
            { *reinterpret_cast<T*>(out) = (*reinterpret_cast<const T*>(lhs)) + (*reinterpret_cast<const T*>(rhs)); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::MathematicsFunc sub()
    {
        if constexpr (has_sub_v<T>)
        {
            return [](void* out, const void* lhs, const void* rhs)
            { *reinterpret_cast<T*>(out) = (*reinterpret_cast<const T*>(lhs)) - (*reinterpret_cast<const T*>(rhs)); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::MathematicsFunc mul()
    {
        if constexpr (has_mul_v<T>)
        {
            return [](void* out, const void* lhs, const void* rhs)
            { *reinterpret_cast<T*>(out) = (*reinterpret_cast<const T*>(lhs)) * (*reinterpret_cast<const T*>(rhs)); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::MathematicsFunc div()
    {
        if constexpr (has_div_v<T>)
        {
            return [](void* out, const void* lhs, const void* rhs)
            { *reinterpret_cast<T*>(out) = (*reinterpret_cast<const T*>(lhs)) / (*reinterpret_cast<const T*>(rhs)); };
        }
        else
        {
            return nullptr;
        }
    }

    // compare
    constexpr static TypeFuncTable::CompareFunc equal()
    {
        if constexpr (has_equal_v<T>)
        {
            return [](const void* lhs, const void* rhs) { return (*reinterpret_cast<const T*>(lhs)) == (*reinterpret_cast<const T*>(rhs)); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::CompareFunc less()
    {
        if constexpr (has_less_v<T>)
        {
            return [](const void* lhs, const void* rhs) { return (*reinterpret_cast<const T*>(lhs)) < (*reinterpret_cast<const T*>(rhs)); };
        }
        else
        {
            return nullptr;
        }
    }
    constexpr static TypeFuncTable::CompareFunc greater()
    {
        if constexpr (has_greater_equal_v<T>)
        {
            return [](const void* lhs, const void* rhs) { return (*reinterpret_cast<const T*>(lhs)) > (*reinterpret_cast<const T*>(rhs)); };
        }
        else
        {
            return nullptr;
        }
    }
};
}// namespace kun

// func table maker impl
namespace kun
{
template<typename T> struct TypeFuncTableMaker : public TypeFuncTableMakerDefault<T>
{
};
}// namespace kun

// make func table
namespace kun
{
template<typename T> void makeTypeFuncTable(TypeFuncTable& table)
{
    table.ctor = TypeFuncTableMaker<T>::ctor();
    table.init = TypeFuncTableMaker<T>::init();
    table.shutdown = TypeFuncTableMaker<T>::shutdown();
    table.dtor = TypeFuncTableMaker<T>::dtor();

    table.serialize = TypeFuncTableMaker<T>::serialize();

    table.copy = TypeFuncTableMaker<T>::copy();
    table.move = TypeFuncTableMaker<T>::move();
    table.assign = TypeFuncTableMaker<T>::assign();
    table.move_assign = TypeFuncTableMaker<T>::move_assign();

    table.swap = TypeFuncTableMaker<T>::swap();
    table.hash = TypeFuncTableMaker<T>::hash();

    table.add = TypeFuncTableMaker<T>::add();
    table.sub = TypeFuncTableMaker<T>::sub();
    table.mul = TypeFuncTableMaker<T>::mul();
    table.div = TypeFuncTableMaker<T>::div();

    table.equal = TypeFuncTableMaker<T>::equal();
    table.less = TypeFuncTableMaker<T>::less();
    table.greater = TypeFuncTableMaker<T>::greater();
}
}// namespace kun