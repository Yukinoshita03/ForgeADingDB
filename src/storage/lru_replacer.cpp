#include "forgeadingdb/storage/lru_replacer.hpp"

namespace forgeadingdb {

LruReplacer::LruReplacer(std::size_t capacity) noexcept : capacity_(capacity) {}

std::size_t LruReplacer::Size() const noexcept { return lru_list_.size(); }
void LruReplacer::Pin(FrameId frame_id) {
  // 1. 检查 frame_id 是否超过 capacity_
  if (frame_id >= capacity_) {
    return;
  }

  auto it = positions_.find(frame_id);
  // 2. 检查 positions_ 中是否已经存在
  if (it == positions_.end()) {
    return;
  }

  lru_list_.erase(it->second);
  // 4. 把返回的迭代器保存到 positions_
  positions_.erase(it);
}

void LruReplacer::Unpin(FrameId frame_id) {
  if (frame_id >= capacity_) {
    return;
  }

  auto it = positions_.find(frame_id);
  if (it != positions_.end()) {
    return;
  }

  auto position = lru_list_.emplace(lru_list_.end(), frame_id);
  positions_.emplace(frame_id, position);
}

std::optional<FrameId> LruReplacer::Victim() {
  // 没有可淘汰的 frame
  if (lru_list_.empty()) {
    return std::nullopt;
  }

  // 链表头部是最旧的 frame
  const FrameId frame_id = lru_list_.front();

  // 从 LRU 链表中删除
  lru_list_.pop_front();

  // 从 frame_id 到链表节点的映射中删除
  positions_.erase(frame_id);

  return frame_id;
}
}  // namespace forgeadingdb
