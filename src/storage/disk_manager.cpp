#include "forgeadingdb/storage/disk_manager.hpp"

#include <cstddef>
#include <ios>
#include <iterator>
#include <stdexcept>
#include <string>

#include "forgeadingdb/common/types.h"
#include "forgeadingdb/storage/page.hpp"

namespace forgeadingdb {

DiskManager::DiskManager(const std::string& file) : file_name_(file) {
  // Let file-stream failures become exceptions handled at the database boundary.
  db_io_.exceptions(std::ios::failbit | std::ios::badbit);

  try {
    db_io_.open(file_name_, std::ios::in | std::ios::out | std::ios::binary);
  } catch (const std::ios_base::failure& error) {
    throw std::runtime_error("Failed to open database file: " + file_name_ +
                             ", reason: " + error.what());
  }
}
//
bool DiskManager::ReadPage(PageId id, Page& page) {
  if (id < 0) {
    return false;
  }

  try {
    // Every page occupies kPageSize consecutive bytes in the database file.
    const std::streamoff offset =
        static_cast<std::streamoff>(id) * static_cast<std::streamoff>(kPageSize);
    db_io_.seekg(offset, std::ios::beg);
    db_io_.read(reinterpret_cast<char*>(page.Data()), static_cast<std::streamsize>(kPageSize));

    page.SetPageId(id);
    return true;

  } catch (const std::ios_base::failure& error) {
    throw std::runtime_error("Failed to read page " + std::to_string(id) + ": " + error.what());
  }
}
bool DiskManager::WritePage(PageId id, const Page& page) {
  if (id < 0) {
    return false;
  }

  try {
    // Position the output stream at the beginning of the target page.
    const std::streamoff offset =
        static_cast<std::streamoff>(id) * static_cast<std::streamoff>(kPageSize);
    db_io_.seekp(offset, std::ios::beg);
    db_io_.write(reinterpret_cast<const char*>(page.Data()), kPageSize);
    db_io_.flush();

    return true;
  } catch (const std::ios_base::failure& error) {
    throw std::runtime_error("Failed to write page " + std::to_string(id) + ": " + error.what());
  }
}
}  // namespace forgeadingdb
