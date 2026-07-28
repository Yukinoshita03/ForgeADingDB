#include "forgeadingdb/storage/page.hpp"
#include <cstddef>
#include "forgeadingdb/common/types.h"

namespace forgeadingdb {

    PageId Page::GetPageId() const noexcept{
        return this->page_id_;
    }
    bool Page::SetPageId(PageId input) noexcept{
    if (input < 0 ){
        return false;
    }else {
        this->page_id_ = input;
    }
    return true;
    };
    std::byte* Page::Data() noexcept {
        return this->data_.begin();
    }
    const std::byte* Page::Data() const noexcept{
        return this->data_.begin();
    }
    bool Page::Reset() noexcept{
        page_id_ = kInvalidPageId;
        this->data_.fill(std::byte{0});
        return true;
    }
// db
}
