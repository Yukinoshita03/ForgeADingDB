#include <iostream>
#include <string_view>

#include "miniredbase/core.h"

int main(int argc, char* argv[]) {
  if (argc == 2 && std::string_view{argv[1]} == "--version") {
    std::cout << "MiniRedBase " << miniredbase::Version() << '\n';
    return 0;
  }

  std::cout << "MiniRedBase is initialized. Database features are not implemented yet.\n";
  return 0;
}

