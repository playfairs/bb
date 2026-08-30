#pragma once

#include <cstdint>
#include <iostream>

namespace bb {

class ProgressIndicator {
 public:
  explicit ProgressIndicator(std::uint64_t total_bytes, std::ostream& stream = std::cerr);

  void update(std::uint64_t completed_bytes);

  void finish();

 private:
  std::uint64_t total_bytes_;
  std::ostream& stream_;
  bool started_{false};
};

}  // namespace bb
