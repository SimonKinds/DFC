#ifndef DFC_PATTERN_RANGE_HPP
#define DFC_PATTERN_RANGE_HPP

#include "raw-pattern.hpp"

namespace dfc {
class PatternRange {
 private:
  int const startInclusive_;
  int const endInclusive_;

 public:
  explicit PatternRange(int const startInclusive,
                        int const endInclusive) noexcept
      : startInclusive_(startInclusive), endInclusive_(endInclusive) {}

  bool includes(RawPattern const& pattern) const {
    auto const size = pattern.size();
    return startInclusive_ <= size && size <= endInclusive_;
  }
};
}  // namespace dfc

#endif
