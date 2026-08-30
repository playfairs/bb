#include "bb/progress.h"

#include <sstream>

namespace bb {

ProgressIndicator::ProgressIndicator(std::uint64_t total_bytes, std::ostream& stream)
    : total_bytes_(total_bytes), stream_(stream) {}

void ProgressIndicator::update(std::uint64_t completed_bytes) {
  if (total_bytes_ == 0) {
    return;
  }
  if (!started_) {
    started_ = true;
    stream_ << "\r[0%]";
  }

  const double ratio = static_cast<double>(completed_bytes) / static_cast<double>(total_bytes_);
  const std::size_t percent = static_cast<std::size_t>(ratio * 100.0);
  std::ostringstream message;
  message << "\r[" << percent << "%]";
  stream_ << message.str();
  stream_.flush();
}

void ProgressIndicator::finish() {
  if (started_) {
    stream_ << "\r[100%]\n";
  }
}

}  // namespace bb
