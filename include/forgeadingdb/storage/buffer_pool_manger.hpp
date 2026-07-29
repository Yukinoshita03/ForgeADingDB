#pragma once
#include <cstddef>
#include <deque>
#include <unordered_map>
#include <vector>

#include "forgeadingdb/common/types.h"
#include "forgeadingdb/storage/disk_manager.hpp"
#include "forgeadingdb/storage/page.hpp"

namespace forgeadingdb {
class BufferPoolManager {
 public:
  BufferPoolManager(std::size_t pool_size, DiskManager& disk_manager);
  Page* FetchPage(PageId page_id);
  bool FlushPage(PageId page_id);

 private:
  std::vector<Page> pages_;
  std::unordered_map<PageId, std::size_t> page_table_;
  std::deque<std::size_t> free_list_;
  DiskManager* disk_manager_{nullptr};
};

}  // namespace forgeadingdb
