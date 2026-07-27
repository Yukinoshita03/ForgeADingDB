#include <cstdlib>

#include "miniredbase/core.h"

int main() {
  if (miniredbase::Version().empty()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

