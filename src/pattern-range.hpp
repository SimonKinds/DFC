#ifndef DFC_PATTERN_RANGE_HPP
#define DFC_PATTERN_RANGE_HPP

#include "pattern.hpp"

namespace dfc {
template <int startInclusive, int endInclusive>
class PatternRange {
 public:
  constexpr inline bool includes(Pattern const& pattern) const noexcept {
    return includes(pattern.size());
  }

  constexpr inline bool includes(RawPattern const& pattern) const noexcept {
    return includes(pattern.size());
  }

 private:
  constexpr inline bool includes(int const size) const noexcept {
    return startInclusive <= size && size <= endInclusive;
  }
};
}  // namespace dfc

#endif
