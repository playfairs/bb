#pragma once

#include <cstdint>
#include <filesystem>

#include "bb/util.h"

namespace bb {

Status ensure_sparse_file(const std::filesystem::path& path, std::uint64_t size_bytes);

}
