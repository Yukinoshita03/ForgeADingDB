#include <iostream>
#include <string_view>

#include "forgeadingdb/core.h"

int main(int argc, char* argv[]) {
  if (argc == 2 && std::string_view{argv[1]} == "--version") {
    std::cout << "ForgeADingDB " << forgeadingdb::Version() << '\n';
    return 0;
  }

  std::cout << "ForgeADingDB is initialized. Database features are not implemented yet.\n";
  return 0;
}

