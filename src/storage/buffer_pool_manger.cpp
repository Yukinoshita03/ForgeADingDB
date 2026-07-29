#include "forgeadingdb/storage/buffer_pool_manger.hpp"

namespace forgeadingdb {

BufferPoolManager::BufferPoolManager(
    std::size_t pool_size,
    DiskManager& disk_manager)
    : pages_(pool_size), disk_manager_(&disk_manager) {}

Page* BufferPoolManager::FetchPage(PageId page_id) {
  auto it = page_table_.find(page_id);
  // find it
  if (it != this->page_table_.end()) {
    return &this->pages_[it->second];
  }
  // TODO....
  return nullptr;
}

}  // namespace forgeadingdb
