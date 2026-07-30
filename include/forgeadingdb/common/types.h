#pragma once

#include <cstddef>
#include <cstdint>

namespace forgeadingdb {

using PageId = std::int64_t;
using FrameId = std::size_t;

constexpr std::size_t kPageSize = 4096;
constexpr PageId kInvalidPageId = -1;

constexpr bool IsValidPageId(PageId page_id) noexcept { return page_id >= 0; }

}  // namespace forgeadingdb
