#ifndef DFC_ON_MATCHER_HPP
#define DFC_ON_MATCHER_HPP

#include "pattern.hpp"

namespace dfc {
template <typename T>
struct OnMatcher {
  inline void onMatch(Pattern const& pattern) const {
    auto const& derived = static_cast<T const&>(*this);
    derived.onMatch(pattern);
  }
};

struct PrintOnMatcher : public OnMatcher<PrintOnMatcher> {
  inline void onMatch(Pattern const& pattern) const {
    printf("(%d)\t\t%*.s\n", pattern.pid(), pattern.size(), pattern.data());
  }
};

}  // namespace dfc

#endif
