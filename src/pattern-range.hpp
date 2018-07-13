#ifndef DFC_PATTERN_RANGE_HPP
#define DFC_PATTERN_RANGE_HPP

#include "pattern.hpp"

namespace dfc {
template <int startInclusive, int endInclusive>
class PatternRange {
 public:
  constexpr inline bool includes(Pattern const& pattern) const noexcept {
    int const size = pattern.size();
    return startInclusive <= size && size <= endInclusive;
  }
};
}  // namespace dfc

#endif
