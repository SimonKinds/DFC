#ifndef DFC_ON_MATCHER_HPP
#define DFC_ON_MATCHER_HPP

#include "pattern.hpp"

namespace dfc {
struct OnMatcher {
  virtual ~OnMatcher() noexcept = default;

  virtual void onMatch(Pattern const& pattern) const = 0;
};

struct PrintOnMatcher : public OnMatcher {
  void onMatch(Pattern const& pattern) const noexcept final override {
    printf("(%d)\t\t%*.s\n", pattern.pid(), pattern.size(), pattern.data());
  }
};

}  // namespace dfc

#endif
