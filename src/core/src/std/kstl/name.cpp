#include "kun/core/std/kstl/name.h"
#include "kun/core/std/kstl/exception.hpp"
#include "kun/core/std/stl.hpp"
#include "kun/core/std/fmt.hpp"

// global data
namespace kun
{
// 4kb per page
static inline constexpr Size NAME_PAGE_SIZE = 4 * 1024;

// name pages
static eastl::vector<UPtr<char>>& namePages()
{
    static eastl::vector<UPtr<char>> instance;
    return instance;
}

// cur page index
static Size& curPageIdx()
{
    // zero means none name
    static Size instance = 1;
    return instance;
}

// name set
static UMap<StringView, Size>& nameIdxMap()
{
    static UMap<StringView, Size> instance;
    return instance;
}

// helper
KUN_INLINE static char* idx2ptr(Size idx)
{
    // get global data
    auto& pages = namePages();

    // calc index
    Size cur_page = idx / NAME_PAGE_SIZE;
    Size cur_page_offset = idx % NAME_PAGE_SIZE;

    return pages[cur_page].get() + cur_page_offset;
}

}// namespace kun

// impl name
namespace kun
{
// ctor
Name::Name(StringView str)
{
    // check len
    if (str.length() > MAX_NAME_LEN)
    {
        throw StrException(format(R"_(name: "{}", len out of range, max is {})_", str, MAX_NAME_LEN));
    }
    else if (str.length() == 0)
    {
        m_idx = 0;
    }

    // get global data
    auto& idx_map = nameIdxMap();
    auto& pages = namePages();
    auto& cur_page_idx = curPageIdx();

    // find name
    auto idx_find_it = idx_map.find(str);
    if (idx_find_it != idx_map.end())
    {
        m_idx = idx_find_it->second;
    }
    else
    {
        // alloc page
        Size max_page_idx = pages.size() * NAME_PAGE_SIZE;
        Size require_len = str.length() + 1;
        Size next_page_idx = cur_page_idx + require_len;
        if (next_page_idx > max_page_idx)
        {
            pages.emplace_back(toUPtr((char*)memory::malloc(NAME_PAGE_SIZE)));
            cur_page_idx = std::max(max_page_idx, cur_page_idx);// prevent first alloc
            next_page_idx = cur_page_idx + require_len;
        }

        // calc dst ptr
        char* dst_ptr = idx2ptr(cur_page_idx);

        // copy data
        *dst_ptr = (char)str.length();
        ++dst_ptr;
        memory::memcpy(dst_ptr, str.data(), str.length());

        // setup idx
        m_idx = cur_page_idx;

        // add to map
        StringView view(dst_ptr, str.length());
        idx_map[view] = m_idx;

        // update cur idx
        cur_page_idx = next_page_idx;
    }
}

// getter
Size Name::length() const { return m_idx ? *idx2ptr(m_idx) : 0; }
StringView Name::str() const
{
    const char* ptr = idx2ptr(m_idx);
    return m_idx ? StringView(ptr + 1, (Size)*ptr) : StringView();
}

}// namespace kun