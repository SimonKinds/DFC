#ifndef DFC_PATTERN_RANGE_HPP
#define DFC_PATTERN_RANGE_HPP

#include "raw-pattern.hpp"

namespace dfc {
template <int startInclusive, int endInclusive>
class PatternRange {
 public:
  constexpr inline bool includes(RawPattern const& pattern) const {
    auto const size = pattern.size();
    return startInclusive <= size && size <= endInclusive;
  }
};
}  // namespace dfc

#endif
