#ifndef DFC_UTIL_BENCHMARK_HPP
#define DFC_UTIL_BENCHMARK_HPP

#include "on-matcher.hpp"

namespace dfc {
namespace benchmark {
struct CountOnMatcher final : public OnMatcher {
  int mutable matchCount = 0;

  void onMatch(dfc::ImmutablePattern const& pattern) const noexcept final {
    (void)(pattern);
    ++matchCount;
  }
};
}  // namespace benchmark
}  // namespace dfc

#endif
