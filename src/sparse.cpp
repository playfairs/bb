#include "bb/sparse.h"

#include <filesystem>
#include <fstream>

namespace bb {

Status ensure_sparse_file(const std::filesystem::path& path, std::uint64_t size_bytes) {
  if (path.parent_path() != ".") {
    std::error_code create_error;
    std::filesystem::create_directories(path.parent_path(), create_error);
    if (create_error) {
      return Status::failure("unable to create parent directory: " + describe_error(create_error));
    }
  }

  std::ofstream stream(path, std::ios::binary | std::ios::trunc);
  if (!stream.is_open()) {
    return Status::failure("unable to create output file");
  }
  stream.close();

  std::error_code resize_error;
  std::filesystem::resize_file(path, size_bytes, resize_error);
  if (resize_error) {
    return Status::failure("unable to resize output file: " + describe_error(resize_error));
  }
  return Status::success();
}

}
