#include <cassert>
#include <cstdint>
#include <string>

#include "bb/filesize.h"

int main() {
  bb::Status status;
  const std::uint64_t bytes = bb::parse_size("1MiB", status);
  assert(status.ok);
  assert(bytes == 1024ULL * 1024ULL);
  assert(bb::parse_size("800MB", status) == 800'000'000ULL);
  assert(status.ok);
  assert(bb::parse_size("1KB", status) == 1000ULL);
  assert(status.ok);
  assert(bb::parse_size("1kb", status) == 125ULL);
  assert(status.ok);
  assert(bb::parse_size("1Kib", status) == 128ULL);
  assert(status.ok);
  assert(bb::parse_size("8b", status) == 1ULL);
  assert(status.ok);
  assert(bb::parse_size("1b", status) == 0ULL);
  assert(!status.ok);
  assert(bb::parse_size("1GB", status) == 1'000'000'000ULL);
  assert(status.ok);
  assert(bb::parse_size("1TB", status) == 1'000'000'000'000ULL);
  assert(status.ok);
  assert(bb::parse_size("800MiB", status) == 800ULL * 1024ULL * 1024ULL);
  assert(status.ok);
  assert(bb::parse_size("1KiB", status) == 1024ULL);
  assert(status.ok);
  assert(bb::parse_size("1GiB", status) == 1024ULL * 1024ULL * 1024ULL);
  assert(status.ok);
  assert(bb::parse_size("1TiB", status) == 1024ULL * 1024ULL * 1024ULL * 1024ULL);
  assert(status.ok);
  assert(bb::parse_size("1PB", status) == 1'000'000'000'000'000ULL);
  assert(status.ok);
  assert(bb::parse_size("1PiB", status) == 1'125'899'906'842'624ULL);
  assert(status.ok);
  assert(bb::parse_size("2PB", status) == 2'000'000'000'000'000ULL);
  assert(status.ok);
  assert(bb::format_size(500'000'000) == "500 MB (500,000,000 bytes)");
  assert(bb::format_size(1536) == "1.536 kB (1,536 bytes)");
  assert(bb::format_size(1) == "1 byte");
  const std::string report = bb::format_size_report(838'860'800ULL);
  assert(report.find("838,860,800 bytes") != std::string::npos);
  assert(report.find("6,710,886,400 bits") != std::string::npos);
  assert(report.find("838.8608 MB") != std::string::npos);
  assert(report.find("800 MiB") != std::string::npos);
  const std::string decimal_report = bb::format_size_report(500'000'000ULL);
  assert(decimal_report.find("476.837158203125 MiB") != std::string::npos);

  assert(bb::parse_size("18446744073709551615MB", status) == 0);
  assert(!status.ok);
  return 0;
}
