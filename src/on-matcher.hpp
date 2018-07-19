#ifndef DFC_ON_MATCHER_HPP
#define DFC_ON_MATCHER_HPP

#include <vector>

#include "immutable-pattern.hpp"

namespace dfc {
struct OnMatcher {
  virtual ~OnMatcher() noexcept = default;

  virtual void onMatch(ImmutablePattern const& pattern) const = 0;
};

struct PrintOnMatcher final : public OnMatcher {
  void onMatch(ImmutablePattern const& pattern) const noexcept {
    printf("(%d)\t\t%*.s\n", pattern.pid(), pattern.size(), pattern.data());
  }
};

struct SaveOnMatcher final : public OnMatcher {
  std::vector<dfc::Pid> mutable matchedPids;

  void onMatch(dfc::ImmutablePattern const& pattern) const {
    matchedPids.emplace_back(pattern.pid());
  }
};

}  // namespace dfc

#endif
