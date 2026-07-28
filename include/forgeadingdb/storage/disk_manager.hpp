#pragma once

#include <fstream>
#include <string>

#include "forgeadingdb/common/types.h"
#include "forgeadingdb/storage/page.hpp"
namespace forgeadingdb {
class DiskManager{
    public:
    explicit DiskManager(const std::string& file);
    bool ReadPage(PageId pageid_,Page & page);
    bool WritePage(PageId pageid_,const Page & page);
    private:
    std::streamoff PageOffset(PageId page_id) const noexcept;
    
    std::fstream db_io_;
    std::string file_name_;
    };
}
