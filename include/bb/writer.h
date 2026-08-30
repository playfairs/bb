#pragma once

#include <cstdint>
#include <filesystem>
#include <ostream>

#include "bb/pattern.h"
#include "bb/util.h"

namespace bb {

struct WriteStatistics {
  std::uint64_t bytes_written{0};
  std::uint64_t chunks_written{0};
};

Status write_file(const std::filesystem::path& path,
                  std::uint64_t size_bytes,
                  PatternKind pattern,
                  std::uint64_t seed,
                  bool sparse,
                  std::ostream* progress_stream = nullptr,
                  WriteStatistics* statistics = nullptr);

}
