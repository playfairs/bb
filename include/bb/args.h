#pragma once

#include <cstdint>
#include <filesystem>

#include "bb/filesize.h"
#include "bb/pattern.h"
#include "bb/util.h"

namespace bb {

enum class Command {
  Create,
  Verify,
};

struct Options {
  Command command{Command::Create};
  std::filesystem::path output_path;
  std::uint64_t size_bytes{0};
  PatternKind pattern{PatternKind::Zero};
  std::uint64_t seed{0xC0FFEE};
  bool sparse{false};
  bool show_progress{true};
  bool verify_after_write{false};
  bool help_requested{false};
};

Status parse_args(int argc, char** argv, Options& options);

}  // namespace bb
