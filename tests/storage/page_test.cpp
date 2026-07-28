#include "forgeadingdb/storage/page.hpp"

#include <cstddef>
#include <cstdlib>
#include "forgeadingdb/common/types.h"
#include <type_traits>
int main() {
  forgeadingdb::Page page;

  if (page.GetPageId() != forgeadingdb::kInvalidPageId) {
    return EXIT_FAILURE;
  }
  {
    forgeadingdb::PageId id = 1;
    page.SetPageId(id);
    if (!page.SetPageId(id)) {
    return EXIT_FAILURE;
    }
    if (page.GetPageId() != id){
      return EXIT_FAILURE;
    }
    if (page.SetPageId(forgeadingdb::kInvalidPageId)) {
    return EXIT_FAILURE;
    }
  }
  {
   page.Reset();
   for (std::size_t index = 0;
     index < forgeadingdb::kPageSize;
     ++index) {
   if (page.Data()[index] != std::byte{0}) {
     return EXIT_FAILURE;
    }
   }
   page.Data()[0] = std::byte{10};
   if (page.Data()[0] != std::byte{10}) {
     return EXIT_FAILURE;
   }
  }
  {
   const forgeadingdb::Page const_page;
   auto result = const_page.Data();

   static_assert(
      std::is_same_v<decltype(result), const std::byte*>
   );
  }
  return EXIT_SUCCESS;
}

