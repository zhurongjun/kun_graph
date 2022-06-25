#pragma once
#include "kun/core/config.h"
#include "kun/core/std/stl.hpp"

namespace kun
{
// RAII lock for scope
// scope must impl lock() & unlock()
template<typename T> class RAII
{
public:
    KUN_INLINE explicit RAII(T&& scope) noexcept
        : m_scope(std::forward<T>(scope))
    {
        if constexpr (std::is_pointer_v<T>)
        {
            if (m_scope)
            {
                m_scope->lock();
            }
        }
        else
        {
            m_scope.lock();
        }
    }
    KUN_INLINE explicit RAII(const T& scope) noexcept
        : m_scope(scope)
    {
        if constexpr (std::is_pointer_v<T>)
        {
            if (m_scope)
            {
                m_scope->lock();
            }
        }
        else
        {
            m_scope.lock();
        }
    }
    KUN_INLINE ~RAII() noexcept
    {
        if constexpr (std::is_pointer_v<T>)
        {
            if (m_scope)
            {
                m_scope->unlock();
            }
        }
        else
        {
            m_scope.unlock();
        }
    }

    RAII(const RAII&) = delete;
    RAII(RAII&&) = delete;
    RAII& operator=(const RAII&) = delete;
    RAII& operator=(RAII&&) = delete;

private:
    T m_scope;
};
}// namespace kun