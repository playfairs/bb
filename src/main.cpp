#include <iostream>

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

  if (options.command == bb::Command::Create) {
    bb::WriteStatistics statistics;
    const bb::Status write_status =
        bb::write_file(options.output_path, options.size_bytes, options.pattern, options.seed,
                       options.sparse, options.show_progress ? &std::cout : nullptr, &statistics);
    if (!write_status.ok) {
      std::cerr << write_status.message << '\n';
      return 1;
    }

    std::cout << "wrote " << bb::format_size(statistics.bytes_written) << " to "
              << options.output_path.string() << '\n';

    if (options.verify_after_write) {
      const bb::VerificationResult verification =
          bb::verify_file(options.output_path, options.size_bytes, options.pattern, options.seed,
                          options.show_progress ? &std::cout : nullptr);
      if (!verification.ok) {
        std::cerr << verification.message << '\n';
        return 1;
      }
      std::cout << "verification succeeded" << '\n';
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

  std::cout << "verified " << bb::format_size(verification.bytes_checked) << " with checksum "
            << verification.checksum << '\n';
  return 0;
}
