#include "bb/writer.h"

#include <fstream>
#include <vector>

#include "bb/progress.h"
#include "bb/sparse.h"

namespace bb {

Status write_file(const std::filesystem::path& path,
                  std::uint64_t size_bytes,
                  PatternKind pattern,
                  std::uint64_t seed,
                  bool sparse,
                  std::ostream* progress_stream,
                  WriteStatistics* statistics) {
  if (path.empty()) {
    return Status::failure("output path must be provided");
  }

  if (path.parent_path() != ".") {
    std::error_code create_error;
    std::filesystem::create_directories(path.parent_path(), create_error);
    if (create_error) {
      return Status::failure("unable to create parent directory: " + describe_error(create_error));
    }
  }

  if (sparse && pattern == PatternKind::Zero) {
    return ensure_sparse_file(path, size_bytes);
  }

  std::ofstream output(path, std::ios::binary | std::ios::trunc);
  if (!output.is_open()) {
    return Status::failure("unable to open output file for writing");
  }

  constexpr std::size_t chunk_size = 1024 * 1024;
  std::vector<std::byte> buffer(chunk_size);
  PatternGenerator generator(pattern, seed);
  ProgressIndicator progress(size_bytes, progress_stream ? *progress_stream : std::cerr);

  std::uint64_t offset = 0;
  while (offset < size_bytes) {
    const std::uint64_t remaining = size_bytes - offset;
    const std::size_t current_chunk = static_cast<std::size_t>(remaining < chunk_size ? remaining : chunk_size);
    generator.fill(std::span<std::byte>(buffer.data(), current_chunk), offset);
    output.write(reinterpret_cast<const char*>(buffer.data()), static_cast<std::streamsize>(current_chunk));
    if (!output) {
      return Status::failure("write failed while producing output data");
    }
    offset += current_chunk;
    if (statistics != nullptr) {
      statistics->bytes_written = offset;
      statistics->chunks_written += 1;
    }
    progress.update(offset);
  }

  output.flush();
  if (!output) {
    return Status::failure("unable to flush output data");
  }
  progress.finish();
  return Status::success();
}

}
