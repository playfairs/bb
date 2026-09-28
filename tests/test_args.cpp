#include <cassert>

#include "bb/args.h"

int main() {
  char program[] = "bb";
  char help[] = "--help";
  char* argv[] = {program, help};

  bb::Options options;
  const bb::Status status = bb::parse_args(2, argv, options);
  assert(status.ok);
  assert(options.help_requested);

  char create[] = "create";
  char create_help[] = "--help";
  char* create_argv[] = {program, create, create_help};
  bb::Options create_options;
  const bb::Status create_status = bb::parse_args(3, create_argv, create_options);
  assert(create_status.ok);
  assert(create_options.help_requested);

  char size[] = "size";
  char path[] = "example.bin";
  char* size_argv[] = {program, size, path};
  bb::Options size_options;
  const bb::Status size_status = bb::parse_args(3, size_argv, size_options);
  assert(size_status.ok);
  assert(size_options.command == bb::Command::Size);
  assert(size_options.file_path == "example.bin");
  return 0;
}
