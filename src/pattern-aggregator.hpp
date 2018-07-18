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
  void add(RawPattern pat);
  std::vector<ImmutablePattern> aggregate();

 private:
  void removeDuplicates();
  std::vector<ImmutablePattern> createPatterns() const;
};
}  // namespace dfc

#endif
