#include "forgeadingdb/storage/lru_replacer.hpp"

#include <cstdlib>

namespace {

bool TestUnpinMakesFrameEvictableOnce() {
  forgeadingdb::LruReplacer replacer(2);

  replacer.Unpin(0);
  if (replacer.Size() != 1) {
    return false;
  }

  replacer.Unpin(0);
  if (replacer.Size() != 1) {
    return false;
  }

  replacer.Unpin(1);
  return replacer.Size() == 2;
}

bool TestVictimReturnsOldestFrame() {
  forgeadingdb::LruReplacer replacer(2);
  replacer.Unpin(0);
  replacer.Unpin(1);

  const auto first_victim = replacer.Victim();
  if (!first_victim.has_value() || first_victim.value() != 0 ||
      replacer.Size() != 1) {
    return false;
  }

  const auto second_victim = replacer.Victim();
  if (!second_victim.has_value() || second_victim.value() != 1 ||
      replacer.Size() != 0) {
    return false;
  }

  return !replacer.Victim().has_value();
}

}  // namespace

int main() {
  return TestUnpinMakesFrameEvictableOnce() && TestVictimReturnsOldestFrame()
             ? EXIT_SUCCESS
             : EXIT_FAILURE;
}
