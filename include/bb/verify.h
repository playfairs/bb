#pragma once

#include <cstdint>
#include <filesystem>
#include <ostream>

#include "bb/pattern.h"
#include "bb/util.h"

namespace bb {

struct VerificationResult {
  bool ok{true};
  std::uint64_t bytes_checked{0};
  std::uint64_t checksum{0};
  std::string message{};
};

VerificationResult verify_file(const std::filesystem::path& path,
                                std::uint64_t expected_size,
                                PatternKind pattern,
                                std::uint64_t seed,
                                std::ostream* progress_stream = nullptr);

}
