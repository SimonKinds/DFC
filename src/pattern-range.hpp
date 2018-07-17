#ifndef DFC_PATTERN_RANGE_HPP
#define DFC_PATTERN_RANGE_HPP

#include "pattern.hpp"

namespace dfc {
template <int startInclusive_, int endInclusive_>
class PatternRange {
 public:
  static int const startInclusive = startInclusive_;
  static int const endInclusive = endInclusive_;

  constexpr inline bool includes(Pattern const& pattern) const noexcept {
    int const size = pattern.size();
    return startInclusive <= size && size <= endInclusive;
  }
};
}  // namespace dfc

#endif
