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
  return 0;
}
