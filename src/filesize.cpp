#include "bb/filesize.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

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

std::string group_digits(std::string value) {
  for (std::size_t position = value.size(); position > 3; position -= 3) {
    value.insert(position - 3, ",");
  }
  return value;
}

void multiply_decimal(std::string& value, unsigned int multiplier) {
  unsigned int carry = 0;
  for (std::size_t index = value.size(); index > 0; --index) {
    const unsigned int product =
        static_cast<unsigned int>(value[index - 1] - '0') * multiplier + carry;
    value[index - 1] = static_cast<char>('0' + product % 10);
    carry = product / 10;
  }
  while (carry > 0) {
    value.insert(value.begin(), static_cast<char>('0' + carry % 10));
    carry /= 10;
  }
}

std::string format_bytes(std::uint64_t bytes) {
  return group_digits(std::to_string(bytes));
}

std::string format_bits(std::uint64_t bytes) {
  std::string bits = std::to_string(bytes);
  multiply_decimal(bits, 8);
  return group_digits(std::move(bits));
}

std::string format_exact_unit(std::uint64_t bytes, std::uint64_t divisor, std::string_view unit) {
  std::uint64_t reduced_divisor = divisor;
  std::size_t twos = 0;
  std::size_t fives = 0;
  while (reduced_divisor % 2 == 0) {
    reduced_divisor /= 2;
    ++twos;
  }
  while (reduced_divisor % 5 == 0) {
    reduced_divisor /= 5;
    ++fives;
  }

  const std::size_t decimal_places = twos > fives ? twos : fives;
  std::string value = std::to_string(bytes);
  for (std::size_t index = twos; index < decimal_places; ++index) {
    multiply_decimal(value, 2);
  }
  for (std::size_t index = fives; index < decimal_places; ++index) {
    multiply_decimal(value, 5);
  }

  if (decimal_places > 0) {
    if (value.size() <= decimal_places) {
      value.insert(0, decimal_places + 1 - value.size(), '0');
    }
    value.insert(value.size() - decimal_places, ".");
    while (value.back() == '0') {
      value.pop_back();
    }
    if (value.back() == '.') {
      value.pop_back();
    }
  }

  const std::size_t decimal_point = value.find('.');
  const std::string integer_part = value.substr(0, decimal_point);
  const std::string fractional_part =
      decimal_point == std::string::npos ? "" : value.substr(decimal_point);
  return group_digits(integer_part) + fractional_part + " " + std::string(unit);
}

constexpr std::array<std::pair<std::string_view, std::uint64_t>, 21> kByteSuffixes = {{
    {"k", 1024ULL},
    {"K", 1024ULL},
    {"m", 1024ULL * 1024ULL},
    {"M", 1024ULL * 1024ULL},
    {"g", 1024ULL * 1024ULL * 1024ULL},
    {"G", 1024ULL * 1024ULL * 1024ULL},
    {"t", 1024ULL * 1024ULL * 1024ULL * 1024ULL},
    {"T", 1024ULL * 1024ULL * 1024ULL * 1024ULL},
    {"p", 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL},
    {"P", 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL},
    {"KiB", 1024ULL},
    {"MiB", 1024ULL * 1024ULL},
    {"GiB", 1024ULL * 1024ULL * 1024ULL},
    {"TiB", 1024ULL * 1024ULL * 1024ULL * 1024ULL},
    {"PiB", 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL},
    {"kB", 1000ULL},
    {"KB", 1000ULL},
    {"MB", 1000ULL * 1000ULL},
    {"GB", 1000ULL * 1000ULL * 1000ULL},
    {"TB", 1000ULL * 1000ULL * 1000ULL * 1000ULL},
    {"PB", 1000ULL * 1000ULL * 1000ULL * 1000ULL * 1000ULL},
}};

constexpr std::array<std::pair<std::string_view, std::uint64_t>, 11> kBitSuffixes = {{
    {"b", 1ULL},
    {"kb", 1000ULL},
    {"Mb", 1000ULL * 1000ULL},
    {"Gb", 1000ULL * 1000ULL * 1000ULL},
    {"Tb", 1000ULL * 1000ULL * 1000ULL * 1000ULL},
    {"Pb", 1000ULL * 1000ULL * 1000ULL * 1000ULL * 1000ULL},
    {"Kib", 1024ULL},
    {"Mib", 1024ULL * 1024ULL},
    {"Gib", 1024ULL * 1024ULL * 1024ULL},
    {"Tib", 1024ULL * 1024ULL * 1024ULL * 1024ULL},
    {"Pib", 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL},
}};

constexpr std::array<std::pair<std::string_view, std::uint64_t>, 5> kDecimalUnits = {{
    {"kB", 1000ULL},
    {"MB", 1000ULL * 1000ULL},
    {"GB", 1000ULL * 1000ULL * 1000ULL},
    {"TB", 1000ULL * 1000ULL * 1000ULL * 1000ULL},
    {"PB", 1000ULL * 1000ULL * 1000ULL * 1000ULL * 1000ULL},
}};

constexpr std::array<std::pair<std::string_view, std::uint64_t>, 5> kBinaryUnits = {{
    {"KiB", 1024ULL},
    {"MiB", 1024ULL * 1024ULL},
    {"GiB", 1024ULL * 1024ULL * 1024ULL},
    {"TiB", 1024ULL * 1024ULL * 1024ULL * 1024ULL},
    {"PiB", 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL},
}};

}  // namespace

std::uint64_t parse_size(std::string_view input, Status& status) {
  status = Status::success();
  std::string value = trim(input);
  if (value.empty()) {
    status = Status::failure("size must not be empty");
    return 0;
  }

  std::size_t index = 0;
  while (index < value.size() && std::isdigit(static_cast<unsigned char>(value[index]))) {
    ++index;
  }

  if (index == 0) {
    status = Status::failure("size must begin with a number");
    return 0;
  }

  std::uint64_t magnitude = 0;
  auto [ptr, ec] = std::from_chars(value.data(), value.data() + index, magnitude);
  if (ec != std::errc{} || ptr != value.data() + index) {
    status = Status::failure("size contains an invalid number");
    return 0;
  }

  const std::string_view suffix(value.data() + index, value.size() - index);
  if (suffix.empty() || suffix == "B") {
    return magnitude;
  }

  const auto byte_suffix =
      std::find_if(kByteSuffixes.begin(), kByteSuffixes.end(),
                   [suffix](const auto& entry) { return entry.first == suffix; });
  if (byte_suffix != kByteSuffixes.end()) {
    if (magnitude > std::numeric_limits<std::uint64_t>::max() / byte_suffix->second) {
      status = Status::failure("size exceeds the supported range");
      return 0;
    }
    return magnitude * byte_suffix->second;
  }

  const auto bit_suffix =
      std::find_if(kBitSuffixes.begin(), kBitSuffixes.end(),
                   [suffix](const auto& entry) { return entry.first == suffix; });
  if (bit_suffix == kBitSuffixes.end()) {
    status = Status::failure("unsupported size suffix");
    return 0;
  }

  if (bit_suffix->second == 1) {
    if (magnitude % 8 != 0) {
      status = Status::failure("bit size must be divisible by 8 to represent whole bytes");
      return 0;
    }
    return magnitude / 8;
  }

  const std::uint64_t byte_multiplier = bit_suffix->second / 8;
  if (magnitude > std::numeric_limits<std::uint64_t>::max() / byte_multiplier) {
    status = Status::failure("size exceeds the supported range");
    return 0;
  }
  return magnitude * byte_multiplier;
}

std::string format_size(std::uint64_t bytes) {
  if (bytes < 1000) {
    return format_bytes(bytes) + (bytes == 1 ? " byte" : " bytes");
  }

  std::size_t unit_index = 0;
  while (unit_index + 1 < kDecimalUnits.size() && bytes >= kDecimalUnits[unit_index + 1].second) {
    ++unit_index;
  }
  const auto& [unit_name, divisor] = kDecimalUnits[unit_index];
  return format_exact_unit(bytes, divisor, unit_name) + " (" + format_bytes(bytes) + " bytes)";
}

std::string format_size_report(std::uint64_t bytes) {
  std::ostringstream report;
  report << "Exact:\n"
         << "  " << format_bytes(bytes) << (bytes == 1 ? " byte\n" : " bytes\n") << "  "
         << format_bits(bytes) << " bits\n"
         << "Decimal (base 1000):";
  for (const auto& [unit_name, divisor] : kDecimalUnits) {
    report << "\n  " << format_exact_unit(bytes, divisor, unit_name);
  }
  report << "\nBinary (base 1024):";
  for (const auto& [unit_name, divisor] : kBinaryUnits) {
    report << "\n  " << format_exact_unit(bytes, divisor, unit_name);
  }
  return report.str();
}

}  // namespace bb
