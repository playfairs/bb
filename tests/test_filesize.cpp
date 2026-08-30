#include <cassert>
#include <cstdint>

#include "bb/filesize.h"

int main() {
  bb::Status status;
  const std::uint64_t bytes = bb::parse_size("1MiB", status);
  assert(status.ok);
  assert(bytes == 1024ULL * 1024ULL);
  assert(bb::format_size(1536) == "1 KiB");
  return 0;
}
