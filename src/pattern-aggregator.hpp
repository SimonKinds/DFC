#ifndef PATTERN_AGGREGATOR_H
#define PATTERN_AGGREGATOR_H

#include <algorithm>
#include <vector>

#include "pattern.hpp"

namespace dfc {
class PatternAggregator {
 private:
  std::vector<RawPattern> patterns_;

 public:
  void add(RawPattern pat) { patterns_.emplace_back(std::move(pat)); }

  const std::vector<RawPattern>& aggregate() {
    removeDuplicates();

    return patterns_;
  }

 private:
  void removeDuplicates() {
    std::sort(std::begin(patterns_), std::end(patterns_));
    patterns_.erase(std::unique(std::begin(patterns_), std::end(patterns_)),
                    std::end(patterns_));
  }
};
}  // namespace dfc

#endif
