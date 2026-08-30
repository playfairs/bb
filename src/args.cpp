#include "bb/args.h"

#include <charconv>
#include <iostream>
#include <string_view>

namespace bb {
namespace {

void print_general_help(const char* program_name) {
  std::cout << "bb - ByteBuilder\n";
  std::cout <<"\n";
  std::cout << "Usage:\n";
  std::cout << "  " << program_name << " <create|verify> [options]\n";
  std::cout << "\n";
  std::cout << "Commands:\n";
  std::cout << "  create    Create a file filled with a chosen pattern\n";
  std::cout << "  verify    Verify that a file matches the expected size and pattern\n";
  std::cout << "\n";
  std::cout << "Global options:\n";
  std::cout << "  --help                Show this help message\n";
  std::cout << "  --output <path>       Output file path\n";
  std::cout << "  --size <size>         File size (for example 1MiB, 512K, 2G)\n";
  std::cout << "  --pattern <name>      Pattern: zero, incrementing, random\n";
  std::cout << "  --seed <value>        Seed for random/incrementing generation\n";
  std::cout << "  --sparse              Create a sparse file when possible\n";
  std::cout << "  --no-progress         Disable progress output\n";
  std::cout << "  --verify              Verify immediately after creating\n";
}

void print_create_help(const char* program_name) {
  std::cout << "Create usage:\n";
  std::cout << "  " << program_name << " create --output <path> --size <size> [options]\n";
  std::cout << "\n";
  std::cout << "Create options:\n";
  std::cout << "  --output <path>       Destination file\n";
  std::cout << "  --size <size>         Output size, such as 1MiB or 512K\n";
  std::cout << "  --pattern <name>      zero, incrementing, or random\n";
  std::cout << "  --seed <value>        Seed for non-zero patterns\n";
  std::cout << "  --sparse              Create a sparse file when possible\n";
  std::cout << "  --no-progress         Disable progress output\n";
  std::cout << "  --verify              Verify the file immediately after writing\n";
}

void print_verify_help(const char* program_name) {
  std::cout << "Verify usage:\n";
  std::cout << "  " << program_name << " verify --output <path> --size <size> [options]\n";
  std::cout << "\n";
  std::cout << "Verify options:\n";
  std::cout << "  --output <path>       File to verify\n";
  std::cout << "  --size <size>         Expected size of the file\n";
  std::cout << "  --pattern <name>      zero, incrementing, or random\n";
  std::cout << "  --seed <value>        Seed used for generation\n";
  std::cout << "  --no-progress         Disable progress output\n";
}

bool parse_uint64(std::string_view input, std::uint64_t& value) {
  const char* begin = input.data();
  const char* end = begin + input.size();
  auto [ptr, ec] = std::from_chars(begin, end, value);
  return ec == std::errc{} && ptr == end;
}

}

Status parse_args(int argc, char** argv, Options& options) {
  if (argc < 2) {
    options.help_requested = true;
    print_general_help(argv[0]);
    return Status::success();
  }

  const std::string_view command_name = argv[1];
  if (command_name == "create") {
    options.command = Command::Create;
  } else if (command_name == "verify") {
    options.command = Command::Verify;
  } else if (command_name == "--help" || command_name == "help" || command_name == "-h") {
    options.help_requested = true;
    print_general_help(argv[0]);
    return Status::success();
  } else {
    print_general_help(argv[0]);
    return Status::failure("unknown subcommand");
  }

  for (int index = 2; index < argc; ++index) {
    const std::string_view arg = argv[index];
    if (arg == "--help" || arg == "-h") {
      options.help_requested = true;
      if (options.command == Command::Create) {
        print_create_help(argv[0]);
      } else {
        print_verify_help(argv[0]);
      }
      return Status::success();
    } else if (arg == "--output") {
      if (index + 1 >= argc) {
        return Status::failure("--output requires a value");
      }
      options.output_path = argv[++index];
    } else if (arg == "--size") {
      if (index + 1 >= argc) {
        return Status::failure("--size requires a value");
      }
      Status size_status;
      options.size_bytes = parse_size(argv[++index], size_status);
      if (!size_status.ok) {
        return size_status;
      }
    } else if (arg == "--pattern") {
      if (index + 1 >= argc) {
        return Status::failure("--pattern requires a value");
      }
      const std::string_view pattern_name = argv[++index];
      if (pattern_name == "zero") {
        options.pattern = PatternKind::Zero;
      } else if (pattern_name == "incrementing") {
        options.pattern = PatternKind::Incrementing;
      } else if (pattern_name == "random") {
        options.pattern = PatternKind::Random;
      } else {
        return Status::failure("unsupported pattern");
      }
    } else if (arg == "--seed") {
      if (index + 1 >= argc) {
        return Status::failure("--seed requires a value");
      }
      std::uint64_t parsed_seed = 0;
      if (!parse_uint64(argv[++index], parsed_seed)) {
        return Status::failure("--seed must be a valid unsigned integer");
      }
      options.seed = parsed_seed;
    } else if (arg == "--sparse") {
      options.sparse = true;
    } else if (arg == "--no-progress") {
      options.show_progress = false;
    } else if (arg == "--verify") {
      options.verify_after_write = true;
    } else {
      return Status::failure(std::string("unrecognized argument: ") + std::string(arg));
    }
  }

  if (options.output_path.empty()) {
    return Status::failure("--output is required");
  }
  if (options.size_bytes == 0) {
    return Status::failure("--size is required and must be greater than zero");
  }
  return Status::success();
}

}
