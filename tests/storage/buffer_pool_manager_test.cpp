#include "forgeadingdb/storage/buffer_pool_manager.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>

namespace {

constexpr const char* kTestDatabaseFile =
    "forgeadingdb_buffer_pool_manager_test.db";
constexpr const char* kEmptyDatabaseFile =
    "forgeadingdb_buffer_pool_manager_empty_test.db";

bool CreateEmptyFile(const char* file_name) {
  std::remove(file_name);
  std::ofstream file(file_name, std::ios::binary);
  return static_cast<bool>(file);
}

void FillPage(forgeadingdb::Page& page,
              forgeadingdb::PageId page_id,
              std::byte value) {
  page.SetPageId(page_id);
  for (std::size_t index = 0; index < forgeadingdb::kPageSize; ++index) {
    page.Data()[index] = value;
  }
}

bool TestFetchCacheHitPoolFullAndFlush() {
  if (!CreateEmptyFile(kTestDatabaseFile)) {
    return false;
  }

  bool passed = false;
  try {
    forgeadingdb::DiskManager disk_manager(kTestDatabaseFile);

    forgeadingdb::Page page_zero;
    forgeadingdb::Page page_one;
    FillPage(page_zero, 0, std::byte{0x11});
    FillPage(page_one, 1, std::byte{0x22});
    disk_manager.WritePage(0, page_zero);
    disk_manager.WritePage(1, page_one);

    forgeadingdb::BufferPoolManager buffer_pool(1, disk_manager);

    if (buffer_pool.FetchPage(forgeadingdb::kInvalidPageId) != nullptr ||
        buffer_pool.FlushPage(forgeadingdb::kInvalidPageId) ||
        buffer_pool.FlushPage(0)) {
      throw std::runtime_error("invalid or uncached page was accepted");
    }

    forgeadingdb::Page* fetched = buffer_pool.FetchPage(0);
    if (fetched == nullptr || fetched->GetPageId() != 0 ||
        fetched->Data()[0] != std::byte{0x11}) {
      throw std::runtime_error("page zero was not loaded correctly");
    }

    fetched->Data()[0] = std::byte{0x7A};
    forgeadingdb::Page* cached = buffer_pool.FetchPage(0);
    if (cached != fetched || cached->Data()[0] != std::byte{0x7A}) {
      throw std::runtime_error("cache hit reloaded or replaced the page");
    }

    if (buffer_pool.FetchPage(1) != nullptr || buffer_pool.FlushPage(1)) {
      throw std::runtime_error("full pool or uncached flush was not rejected");
    }

    if (!buffer_pool.FlushPage(0)) {
      throw std::runtime_error("cached page could not be flushed");
    }

    forgeadingdb::Page persisted;
    disk_manager.ReadPage(0, persisted);
    if (persisted.GetPageId() != 0 ||
        persisted.Data()[0] != std::byte{0x7A} ||
        persisted.Data()[1] != std::byte{0x11}) {
      throw std::runtime_error("flushed page was not persisted");
    }

    forgeadingdb::BufferPoolManager all_pages_pool(2, disk_manager);
    forgeadingdb::Page* fetched_zero = all_pages_pool.FetchPage(0);
    forgeadingdb::Page* fetched_one = all_pages_pool.FetchPage(1);
    if (fetched_zero == nullptr || fetched_one == nullptr) {
      throw std::runtime_error("pages were not loaded for flush-all");
    }

    fetched_zero->Data()[0] = std::byte{0x3C};
    fetched_one->Data()[0] = std::byte{0x4D};
    if (!all_pages_pool.FlushAllPage()) {
      throw std::runtime_error("cached pages could not all be flushed");
    }

    forgeadingdb::Page persisted_zero;
    forgeadingdb::Page persisted_one;
    disk_manager.ReadPage(0, persisted_zero);
    disk_manager.ReadPage(1, persisted_one);
    if (persisted_zero.Data()[0] != std::byte{0x3C} ||
        persisted_one.Data()[0] != std::byte{0x4D}) {
      throw std::runtime_error("flush-all did not persist every cached page");
    }

    forgeadingdb::BufferPoolManager empty_pool(0, disk_manager);
    if (empty_pool.FetchPage(0) != nullptr) {
      throw std::runtime_error("zero-sized pool returned a page");
    }

    passed = true;
  } catch (...) {
    passed = false;
  }

  std::remove(kTestDatabaseFile);
  return passed;
}

bool TestReadErrorPropagation() {
  if (!CreateEmptyFile(kEmptyDatabaseFile)) {
    return false;
  }

  bool propagated = false;
  try {
    forgeadingdb::DiskManager disk_manager(kEmptyDatabaseFile);
    forgeadingdb::BufferPoolManager buffer_pool(1, disk_manager);

    try {
      // Page zero does not exist in the empty file, so DiskManager must throw.
      buffer_pool.FetchPage(0);
    } catch (const std::runtime_error&) {
      propagated = true;
    }
  } catch (...) {
    propagated = false;
  }

  std::remove(kEmptyDatabaseFile);
  return propagated;
}

}  // namespace

int main() {
  if (!TestFetchCacheHitPoolFullAndFlush() || !TestReadErrorPropagation()) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
