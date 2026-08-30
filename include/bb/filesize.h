#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "bb/util.h"

namespace bb {

std::uint64_t parse_size(std::string_view input, Status& status);

std::string format_size(std::uint64_t bytes);

}  // namespace bb
