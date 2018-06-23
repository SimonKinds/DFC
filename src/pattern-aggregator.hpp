#ifndef DFC_PATTERN_AGGREGATOR_HPP
#define DFC_PATTERN_AGGREGATOR_HPP

#include <algorithm>
#include <vector>

#include "pattern.hpp"

namespace dfc {
class PatternAggregator {
 private:
  std::vector<RawPattern> patterns_;

 public:
  void add(RawPattern pat) { patterns_.emplace_back(std::move(pat)); }

  std::vector<Pattern> aggregate() {
    removeDuplicates();

    std::vector<Pattern> patterns = createPatterns();

    resetPatterns();

    return patterns;
  }

 private:
  void removeDuplicates() {
    std::sort(std::begin(patterns_), std::end(patterns_));
    patterns_.erase(std::unique(std::begin(patterns_), std::end(patterns_)),
                    std::end(patterns_));
  }

  std::vector<Pattern> createPatterns() {
    std::vector<Pattern> newPatterns;
    newPatterns.reserve(patterns_.size());

    Pid pid = 0;
    for (auto&& pattern : patterns_) {
      newPatterns.emplace_back(pid, std::move(pattern));
      ++pid;
    }

    return newPatterns;
  }

  void resetPatterns() { patterns_.clear(); }
};
}  // namespace dfc

#endif
