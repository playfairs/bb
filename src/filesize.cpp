#include "bb/filesize.h"

#include <array>
#include <charconv>
#include <cctype>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

namespace bb {
namespace {

std::string trim(std::string_view input) {
  std::size_t begin = 0;
  while (begin < input.size() && std::isspace(static_cast<unsigned char>(input[begin]))) {
    ++begin;
  }
  std::size_t end = input.size();
  while (end > begin && std::isspace(static_cast<unsigned char>(input[end - 1]))) {
    --end;
  }
  return std::string(input.substr(begin, end - begin));
}

}

std::uint64_t parse_size(std::string_view input, Status& status) {
  status = Status::success();
  std::string value = trim(input);
  if (value.empty()) {
    status = Status::failure("size must not be empty");
    return 0;
  }

  std::string lower = to_lower(value);
  std::uint64_t multiplier = 1;
  std::size_t index = 0;
  while (index < lower.size() && std::isdigit(static_cast<unsigned char>(lower[index]))) {
    ++index;
  }

  if (index == 0) {
    status = Status::failure("size must begin with a number");
    return 0;
  }

  std::uint64_t magnitude = 0;
  auto [ptr, ec] = std::from_chars(lower.data(), lower.data() + index, magnitude);
  if (ec != std::errc{} || ptr != lower.data() + index) {
    status = Status::failure("size contains an invalid number");
    return 0;
  }

  std::string suffix = lower.substr(index);
  if (suffix.empty() || suffix == "b") {
    return magnitude;
  }

  if (suffix == "k" || suffix == "kb" || suffix == "kib") {
    multiplier = 1024ULL;
  } else if (suffix == "m" || suffix == "mb" || suffix == "mib") {
    multiplier = 1024ULL * 1024ULL;
  } else if (suffix == "g" || suffix == "gb" || suffix == "gib") {
    multiplier = 1024ULL * 1024ULL * 1024ULL;
  } else if (suffix == "t" || suffix == "tb" || suffix == "tib") {
    multiplier = 1024ULL * 1024ULL * 1024ULL * 1024ULL;
  } else {
    status = Status::failure("unsupported size suffix");
    return 0;
  }

  if (magnitude > std::numeric_limits<std::uint64_t>::max() / multiplier) {
    status = Status::failure("size exceeds the supported range");
    return 0;
  }

  return magnitude * multiplier;
}

std::string format_size(std::uint64_t bytes) {
  constexpr std::array<const char*, 5> units = {{"B", "KiB", "MiB", "GiB", "TiB"}};
  double value = static_cast<double>(bytes);
  std::size_t unit_index = 0;
  while (value >= 1024.0 && unit_index + 1 < units.size()) {
    value /= 1024.0;
    ++unit_index;
  }
  if (unit_index == 0) {
    return std::to_string(bytes) + " B";
  }
  return std::to_string(static_cast<unsigned long long>(value)) + " " + units[unit_index];
}

}
