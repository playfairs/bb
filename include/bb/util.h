#pragma once

#include <filesystem>
#include <string>

namespace bb {

struct Status {
  bool ok{true};
  std::string message{};

  static Status success();
  static Status failure(std::string message);
};

std::string describe_error(const std::error_code& error_code);
std::string to_lower(std::string value);

}
