#include "forgeadingdb/storage/disk_manager.hpp"

#include <cstdio>
#include <cstdlib>
#include <fstream>

namespace {

constexpr const char* kTestDatabaseFile = "forgeadingdb_disk_manager_test.db";

bool PagesEqual(const forgeadingdb::Page& lhs,
                const forgeadingdb::Page& rhs) {
  if (lhs.GetPageId() != rhs.GetPageId()) {
    return false;
  }

  for (std::size_t index = 0; index < forgeadingdb::kPageSize; ++index) {
    if (lhs.Data()[index] != rhs.Data()[index]) {
      return false;
    }
  }

  return true;
}

}  // namespace

int main() {
  // DiskManager opens an existing file in read/write mode.
  std::remove(kTestDatabaseFile);

  {
    std::ofstream create_file(kTestDatabaseFile, std::ios::binary);
    if (!create_file) {
      return EXIT_FAILURE;
    }
  }

  forgeadingdb::Page page_zero;
  forgeadingdb::Page page_one;
  page_zero.SetPageId(0);
  page_one.SetPageId(1);

  for (std::size_t index = 0; index < forgeadingdb::kPageSize; ++index) {
    page_zero.Data()[index] = std::byte{0x11};
    page_one.Data()[index] = std::byte{0x22};
  }

  try {
    forgeadingdb::DiskManager disk_manager(kTestDatabaseFile);

    if (!disk_manager.WritePage(0, page_zero) ||
        !disk_manager.WritePage(1, page_one)) {
      std::remove(kTestDatabaseFile);
      return EXIT_FAILURE;
    }

    forgeadingdb::Page loaded_zero;
    forgeadingdb::Page loaded_one;

    if (!disk_manager.ReadPage(0, loaded_zero) ||
        !disk_manager.ReadPage(1, loaded_one)) {
      std::remove(kTestDatabaseFile);
      return EXIT_FAILURE;
    }

    if (!PagesEqual(page_zero, loaded_zero) ||
        !PagesEqual(page_one, loaded_one)) {
      std::remove(kTestDatabaseFile);
      return EXIT_FAILURE;
    }

    // Invalid page IDs must be rejected before touching the file stream.
    if (disk_manager.ReadPage(forgeadingdb::kInvalidPageId, loaded_zero) ||
        disk_manager.WritePage(forgeadingdb::kInvalidPageId, page_zero)) {
      std::remove(kTestDatabaseFile);
      return EXIT_FAILURE;
    }
  } catch (...) {
    std::remove(kTestDatabaseFile);
    return EXIT_FAILURE;
  }

  std::remove(kTestDatabaseFile);
  return EXIT_SUCCESS;
}
