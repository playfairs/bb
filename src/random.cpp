#include "bb/random.h"

#include <charconv>
#include <string>

namespace bb {

std::uint64_t make_seed(std::string_view input, std::uint64_t fallback) {
  if (input.empty()) {
    return fallback;
  }
  std::uint64_t value = fallback;
  auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), value);
  if (ec == std::errc{} && ptr == input.data() + input.size()) {
    return value;
  }
  return fallback;
}

std::uint64_t next_random(std::uint64_t& state) {
  state = state * 6364136223846793005ULL + 1442695040888963407ULL;
  return state;
}

}
