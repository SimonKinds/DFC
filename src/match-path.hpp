#ifndef DFC_MATCH_PATH_HPP
#define DFC_MATCH_PATH_HPP

#include <utility>

#include "byte.hpp"
#include "df.hpp"
#include "is-direct-filter.hpp"
#include "on-matcher.hpp"

namespace dfc {

template <typename DF, typename CT>
class MatchPath {
  static_assert(is_direct_filter<DF>::value,
                "First template parameter must be a direct filter");

 private:
  DF df_;
  CT ct_;

 public:
  void addPattern(ImmutablePattern const& pattern) {
    df_.addPattern(pattern);
    ct_.addPattern(pattern);
  }

  inline void match(char const* const in, int const remaining,
                    OnMatcher const& onMatcher) const {
    match(reinterpret_cast<byte const*>(in), remaining, onMatcher);
  }

  inline void match(byte const* const in, int const remaining,
                    OnMatcher const& onMatcher) const {
    if (df_.contains(in)) {
      ct_.findAllMatches(in, remaining, onMatcher);
    }
  }
};
}  // namespace dfc

#endif
