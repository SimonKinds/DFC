#include "pattern-aggregator.hpp"

#include <algorithm>

namespace dfc {
void PatternAggregator::add(RawPattern pat) {
  patterns_.emplace_back(std::move(pat));
}

std::vector<ImmutablePattern> PatternAggregator::aggregate() {
  removeDuplicates();

  return createPatterns();
}

void PatternAggregator::removeDuplicates() {
  std::sort(std::begin(patterns_), std::end(patterns_));
  patterns_.erase(std::unique(std::begin(patterns_), std::end(patterns_)),
                  std::end(patterns_));
}

std::vector<ImmutablePattern> PatternAggregator::createPatterns() const {
  std::vector<ImmutablePattern> newPatterns;
  newPatterns.reserve(patterns_.size());

  Pid pid{0};
  for (auto&& pattern : patterns_) {
    newPatterns.emplace_back(pid, pattern);
    pid = pid.next();
  }

  return newPatterns;
}
}  // namespace dfc
