#include "forgeadingdb/storage/lru_replacer.hpp"

namespace forgeadingdb {

LruReplacer::LruReplacer(std::size_t capacity) noexcept : capacity_(capacity) {}

std::size_t LruReplacer::Size() const noexcept { return lru_list_.size(); }

}  // namespace forgeadingdb
