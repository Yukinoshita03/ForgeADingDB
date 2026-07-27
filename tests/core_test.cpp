#include <cstdlib>

#include "forgeadingdb/core.h"

int main() {
  if (forgeadingdb::Version().empty()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

