#include <array>
#include <cassert>

#include "bb/pattern.h"

int main() {
  bb::PatternGenerator generator(bb::PatternKind::Zero);
  std::array<std::byte, 4> buffer{};
  generator.fill(buffer, 0);
  assert(buffer[0] == std::byte{0});
  assert(buffer[3] == std::byte{0});

  bb::PatternGenerator incrementing(bb::PatternKind::Incrementing);
  std::array<std::byte, 4> incremented{};
  incrementing.fill(incremented, 0);
  assert(incremented[0] == std::byte{0});
  assert(incremented[3] == std::byte{3});
  return 0;
}
