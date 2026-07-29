#include "forgeadingdb/storage/buffer_pool_manager.hpp"

#include <cstddef>

#include "forgeadingdb/common/types.h"

namespace forgeadingdb {

BufferPoolManager::BufferPoolManager(std::size_t pool_size, DiskManager& disk_manager)
    : pages_(pool_size), frame_metadata_(pool_size), disk_manager_(&disk_manager) {
  for (std::size_t frame_id = 0; frame_id < pool_size; ++frame_id) {
    free_frame_ids_.push_back(frame_id);
  }
}

Page* BufferPoolManager::FetchPage(PageId page_id) {
  if (page_id < 0) {
    return nullptr;
  }

  const auto page_table_entry = page_table_.find(page_id);
  if (page_table_entry != page_table_.end()) {
    const std::size_t frame_id = page_table_entry->second;
    frame_metadata_[frame_id].pin_count++;
    return &pages_[frame_id];
  }

  if (free_frame_ids_.empty()) {
    bool victim_found = false;
    for (std::size_t frame_id = 0; frame_id < frame_metadata_.size(); ++frame_id) {
      FrameMetadata& metadata = frame_metadata_[frame_id];
      // A pinned frame is still in use and must never be replaced.
      if (metadata.pin_count != 0) {
        continue;
      }

      const PageId victim_page_id = pages_[frame_id].GetPageId();
      // Only dirty pages contain changes that have not reached disk.
      if (metadata.is_dirty && !disk_manager_->WritePage(victim_page_id, pages_[frame_id])) {
        return nullptr;
      }
      page_table_.erase(victim_page_id);

      pages_[frame_id].Reset();
      metadata = {};
      free_frame_ids_.push_back(frame_id);

      victim_found = true;
      break;
    }

    if (!victim_found) {
      return nullptr;
    }
  }

  const std::size_t frame_id = free_frame_ids_.front();
  Page& page = pages_[frame_id];
  if (!disk_manager_->ReadPage(page_id, page)) {
    return nullptr;
  }

  free_frame_ids_.pop_front();
  page_table_.emplace(page_id, frame_id);
  frame_metadata_[frame_id].pin_count = 1;
  return &page;
}

bool BufferPoolManager::FlushPage(PageId page_id) {
  if (page_id < 0) {
    return false;
  }
  const auto page_table_entry = page_table_.find(page_id);
  if (page_table_entry == page_table_.end()) {
    return false;
  }

  const std::size_t frame_id = page_table_entry->second;
  if (disk_manager_->WritePage(page_id, pages_[frame_id])) {
    frame_metadata_[frame_id].is_dirty = false;
    return true;
  }
  return false;
}

bool BufferPoolManager::FlushAllPages() {
  for (const auto& [page_id, frame_id] : page_table_) {
    if (!disk_manager_->WritePage(page_id, pages_[frame_id])) {
      return false;
    }
    frame_metadata_[frame_id].is_dirty = false;
  }

  return true;
}

bool BufferPoolManager::UnpinPage(PageId page_id, bool is_dirty) {
  if (page_id < 0) {
    return false;
  }

  const auto page_table_entry = page_table_.find(page_id);
  if (page_table_entry == page_table_.end()) {
    return false;
  }

  const std::size_t frame_id = page_table_entry->second;
  FrameMetadata& metadata = frame_metadata_[frame_id];
  if (metadata.pin_count == 0) {
    return false;
  }

  // A later read-only user must not erase an earlier dirty state.
  if (is_dirty) {
    metadata.is_dirty = true;
  }

  metadata.pin_count--;
  return true;
}
}  // namespace forgeadingdb
