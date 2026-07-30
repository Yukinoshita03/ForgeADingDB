#pragma once

#include <cstddef>
#include <list>
#include <optional>
#include <unordered_map>

#include "forgeadingdb/common/types.h"

namespace forgeadingdb {

class LruReplacer {
 public:
  explicit LruReplacer(std::size_t capacity) noexcept;

  void Pin(FrameId frame_id);
  void Unpin(FrameId frame_id);
  [[nodiscard]] std::optional<FrameId> Victim();
  [[nodiscard]] std::size_t Size() const noexcept;

 private:
  std::size_t capacity_;
  // Front is least recently used; back is most recently used.
  std::list<FrameId> lru_list_;
  // Maps each evictable frame to its node in lru_list_.
  std::unordered_map<FrameId, std::list<FrameId>::iterator> positions_;
};

}  // namespace forgeadingdb
