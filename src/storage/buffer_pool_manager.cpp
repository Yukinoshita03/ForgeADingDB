#include <cstddef>
#include "forgeadingdb/common/types.h"

#include "forgeadingdb/storage/buffer_pool_manager.hpp"

namespace forgeadingdb {

BufferPoolManager::BufferPoolManager(
    std::size_t pool_size,
    DiskManager& disk_manager)
    : pages_(pool_size),frame_metadata_(pool_size),disk_manager_(&disk_manager) {
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
    const std::size_t index = it->second;
    frame_metadata_[index].pin_count++;
    return &pages_[index];
  }

  if (free_list_.empty()) {
    for(auto i = this->frame_metadata_.begin();
    i != this->frame_metadata_.end();++i){
      if(it->second== 0){

      }
    }
    return nullptr;
  }
  const std::size_t index = free_list_.front();
  Page& page = pages_[index];
  if (!disk_manager_->ReadPage(page_id, page)) {
    return nullptr;
  }
  
  free_list_.pop_front();
  page_table_.emplace(page_id, index);
  this->frame_metadata_[index].pin_count++;
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
bool BufferPoolManager::UnpinPage(PageId page_id,bool is_dirty){
  (void)is_dirty;
  // 1. 非法 page_id 返回 false 
  if(page_id < 0){
    return false;
  }
  // 2. 在 page_table_ 中查找 page_id
  auto it = page_table_.find(page_id);
  if(it == page_table_.end()){
    return false;
  }

  // 3. 找到 page 所在的 frame
  const std::size_t index = it->second;

  // 4. pin_count 已经为 0，不能继续减少
  if(frame_metadata_[index].pin_count == 0){
    return false;
  }

  // 5. 释放一次对该 page 的引用
  frame_metadata_[index].pin_count--;
  return true;
}
}  // namespace forgeadingdb
