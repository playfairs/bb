#include "bb/verify.h"

#include <fstream>
#include <vector>

#include "bb/progress.h"

namespace bb {

VerificationResult verify_file(const std::filesystem::path& path, std::uint64_t expected_size,
                               PatternKind pattern, std::uint64_t seed,
                               std::ostream* progress_stream) {
  VerificationResult result;
  if (!std::filesystem::exists(path)) {
    result.ok = false;
    result.message = "file does not exist";
    return result;
  }

  std::ifstream input(path, std::ios::binary);
  if (!input.is_open()) {
    result.ok = false;
    result.message = "unable to open file for verification";
    return result;
  }

  std::error_code size_error;
  const std::uintmax_t actual_size = std::filesystem::file_size(path, size_error);
  if (size_error) {
    result.ok = false;
    result.message = "unable to stat file: " + describe_error(size_error);
    return result;
  }
  if (actual_size != expected_size) {
    result.ok = false;
    result.message = "size mismatch";
    return result;
  }

  constexpr std::size_t chunk_size = 1024 * 1024;
  std::vector<std::byte> expected_buffer(chunk_size);
  std::vector<char> actual_buffer(chunk_size);
  PatternGenerator generator(pattern, seed);
  ProgressIndicator progress(expected_size, progress_stream ? *progress_stream : std::cerr);
  std::uint64_t offset = 0;
  std::uint64_t checksum = 14695981039346656037ULL;

  while (input) {
    input.read(actual_buffer.data(), static_cast<std::streamsize>(actual_buffer.size()));
    const std::streamsize bytes_read = input.gcount();
    if (bytes_read == 0) {
      break;
    }

    generator.fill(
        std::span<std::byte>(expected_buffer.data(), static_cast<std::size_t>(bytes_read)), offset);
    for (std::streamsize index = 0; index < bytes_read; ++index) {
      const std::byte expected = expected_buffer[static_cast<std::size_t>(index)];
      const char actual = actual_buffer[static_cast<std::size_t>(index)];
      checksum ^= static_cast<unsigned char>(expected);
      checksum *= 1099511628211ULL;
      if (actual != static_cast<char>(expected)) {
        result.ok = false;
        result.message = "content mismatch at byte " + std::to_string(offset + index);
        result.bytes_checked = offset + index;
        return result;
      }
    }

    offset += static_cast<std::uint64_t>(bytes_read);
    result.bytes_checked += static_cast<std::uint64_t>(bytes_read);
    progress.update(offset);
  }

  progress.finish();
  result.checksum = checksum;
  result.message = "verification succeeded";
  return result;
}

}  // namespace bb
