#include "bb/pattern.h"

#include <random>

namespace bb {

PatternGenerator::PatternGenerator(PatternKind kind, std::uint64_t seed)
    : kind_(kind), seed_(seed) {}

void PatternGenerator::fill(std::span<std::byte> buffer, std::uint64_t offset) const {
  switch (kind_) {
    case PatternKind::Zero:
      for (std::size_t index = 0; index < buffer.size(); ++index) {
        buffer[index] = std::byte{0};
      }
      break;
    case PatternKind::Incrementing:
      for (std::size_t index = 0; index < buffer.size(); ++index) {
        buffer[index] = std::byte{static_cast<unsigned char>((offset + index) % 256)};
      }
      break;
    case PatternKind::Random: {
      std::mt19937_64 engine(seed_ + offset);
      std::uniform_int_distribution<int> distribution(0, 255);
      for (std::size_t index = 0; index < buffer.size(); ++index) {
        buffer[index] = std::byte{static_cast<unsigned char>(distribution(engine))};
      }
      break;
    }
  }
}

const char* to_string(PatternKind pattern) {
  switch (pattern) {
    case PatternKind::Zero:
      return "zero";
    case PatternKind::Incrementing:
      return "incrementing";
    case PatternKind::Random:
      return "random";
  }
  return "unknown";
}

}  // namespace bb
