#include <cassert>
#include <cstdint>

#include "bb/random.h"

int main() {
  const std::uint64_t seed = bb::make_seed("12345", 7);
  assert(seed == 12345ULL);
  std::uint64_t state = seed;
  assert(bb::next_random(state) != bb::next_random(state));
  return 0;
}
