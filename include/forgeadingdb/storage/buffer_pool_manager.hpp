#pragma once
#include <atomic>
#include <cstddef>
#include <deque>
#include <unordered_map>
#include <vector>

#include "forgeadingdb/common/types.h"
#include "forgeadingdb/storage/disk_manager.hpp"
#include "forgeadingdb/storage/page.hpp"

namespace forgeadingdb {
class BufferPoolManager {
 private:

   struct FrameMetadata{
    std::size_t pin_count{0};
  };

  std::vector<Page> pages_;
  std::vector<FrameMetadata> frame_metadata_;
  std::unordered_map<PageId, std::size_t> page_table_;
  std::deque<std::size_t> free_list_;
  DiskManager* disk_manager_{nullptr};

 public:

  BufferPoolManager(std::size_t pool_size, DiskManager& disk_manager);
  // Returns nullptr for invalid IDs or a full pool; propagates disk I/O errors.
  Page* FetchPage(PageId page_id);
  // Returns false for invalid or uncached IDs; propagates disk I/O errors.
  bool FlushPage(PageId page_id);
  // Flushes all cached pages; propagates disk I/O errors.
  bool FlushAllPage();
  bool UnpinPage(PageId page_id, bool is_dirty);
};

}  // namespace forgeadingdb
