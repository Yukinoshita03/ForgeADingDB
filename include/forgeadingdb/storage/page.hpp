#pragma once

#include <array>
#include <cstddef>

#include "forgeadingdb/common/types.h"

namespace forgeadingdb {

class Page {
 private:
  PageId page_id_{kInvalidPageId};
  std::array<std::byte, kPageSize> data_{};

 public:
  Page() = default;

  PageId GetPageId() const noexcept;
  bool SetPageId(PageId input) noexcept;

  std::byte* Data() noexcept;
  const std::byte* Data() const noexcept;
  bool Reset() noexcept;

};

}  // namespace forgeadingdb
