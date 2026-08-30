#include "bb/util.h"

#include <system_error>

namespace bb {

Status Status::success() {
  return {};
}

Status Status::failure(std::string message) {
  Status status;
  status.ok = false;
  status.message = std::move(message);
  return status;
}

std::string describe_error(const std::error_code& error_code) {
  if (error_code) {
    return error_code.message();
  }
  return "success";
}

std::string to_lower(std::string value) {
  for (char& ch : value) {
    if (ch >= 'A' && ch <= 'Z') {
      ch = static_cast<char>(ch - 'A' + 'a');
    }
  }
  return value;
}

}
