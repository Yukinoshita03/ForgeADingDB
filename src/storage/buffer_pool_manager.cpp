#include <cstddef>

#include "forgeadingdb/storage/buffer_pool_manager.hpp"

namespace forgeadingdb {

BufferPoolManager::BufferPoolManager(
    std::size_t pool_size,
    DiskManager& disk_manager)
    : pages_(pool_size), disk_manager_(&disk_manager) {
  for (std::size_t index = 0; index < pool_size; ++index) {
    free_list_.push_back(index);
  }
}

Page* BufferPoolManager::FetchPage(PageId page_id) {
  if (page_id < 0) {
    return nullptr;
  }

  auto it = page_table_.find(page_id);
  if (it != page_table_.end()) {
    return &pages_[it->second];
  }

  if (free_list_.empty()) {
    return nullptr;
  }
  const std::size_t index = free_list_.front();
  Page& page = pages_[index];
  if (!disk_manager_->ReadPage(page_id, page)) {
    return nullptr;
  }

  free_list_.pop_front();
  page_table_.emplace(page_id, index);
  return &page;
}

bool BufferPoolManager::FlushPage(PageId page_id) {
  if (page_id < 0) {
    return false;
  }
  auto it = page_table_.find(page_id);
  if (it == page_table_.end()) {
    return false;
  }

  return disk_manager_->WritePage(page_id, pages_[it->second]);
}

bool BufferPoolManager::FlushAllPage() {
  for (const auto& [page_id, index] : page_table_) {
    if (!disk_manager_->WritePage(page_id, pages_[index])) {
      return false;
    }
  }

  return true;
}

}  // namespace forgeadingdb
