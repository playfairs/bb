#include <cstdint>
#include <filesystem>
#include <iostream>
#include <limits>

#include "bb/args.h"
#include "bb/verify.h"
#include "bb/writer.h"

int main(int argc, char** argv) {
  bb::Options options;
  const bb::Status parse_status = bb::parse_args(argc, argv, options);
  if (!parse_status.ok) {
    std::cerr << parse_status.message << '\n';
    return 1;
  }
  if (options.help_requested) {
    return 0;
  }

  if (options.command == bb::Command::Size) {
    std::error_code size_error;
    const std::uintmax_t file_size = std::filesystem::file_size(options.file_path, size_error);
    if (size_error) {
      std::cerr << "unable to read file size: " << size_error.message() << '\n';
      return 1;
    }
    if (file_size > std::numeric_limits<std::uint64_t>::max()) {
      std::cerr << "file size exceeds the supported range\n";
      return 1;
    }
    std::cout << "File: " << options.file_path.string() << '\n'
              << bb::format_size_report(static_cast<std::uint64_t>(file_size)) << '\n';
    return 0;
  }

  if (options.command == bb::Command::Create) {
    bb::WriteStatistics statistics;
    const bb::Status write_status =
        bb::write_file(options.output_path, options.size_bytes, options.pattern, options.seed,
                       options.sparse, options.show_progress ? &std::cout : nullptr, &statistics);
    if (!write_status.ok) {
      std::cerr << write_status.message << '\n';
      return 1;
    }

    std::cout << "Wrote " << bb::format_size(statistics.bytes_written) << " to "
              << options.output_path.string() << '\n';

    if (options.verify_after_write) {
      const bb::VerificationResult verification =
          bb::verify_file(options.output_path, options.size_bytes, options.pattern, options.seed,
                          options.show_progress ? &std::cout : nullptr);
      if (!verification.ok) {
        std::cerr << verification.message << '\n';
        return 1;
      }
      std::cout << "Verification succeeded." << '\n';
    }
    return 0;
  }

  const bb::VerificationResult verification =
      bb::verify_file(options.output_path, options.size_bytes, options.pattern, options.seed,
                      options.show_progress ? &std::cout : nullptr);
  if (!verification.ok) {
    std::cerr << verification.message << '\n';
    return 1;
  }

  std::cout << "Verified " << bb::format_size(verification.bytes_checked) << " with checksum "
            << verification.checksum << '\n';
  return 0;
}
