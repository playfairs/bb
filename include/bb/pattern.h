#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace bb {

enum class PatternKind {
  Zero,
  Incrementing,
  Random,
};

class PatternGenerator {
 public:
  explicit PatternGenerator(PatternKind kind, std::uint64_t seed = 0);

  void fill(std::span<std::byte> buffer, std::uint64_t offset) const;

 private:
  PatternKind kind_;
  std::uint64_t seed_;
};

const char* to_string(PatternKind pattern);

}  // namespace bb
