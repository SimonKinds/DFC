#ifndef DFC_MATCH_PATH_HPP
#define DFC_MATCH_PATH_HPP

#include <utility>
#include "byte.hpp"
#include "on-matcher.hpp"

namespace dfc {
template <typename DF, typename CT>
class MatchPath {
 private:
  DF const df_;
  CT const ct_;

 public:
  explicit MatchPath(DF df, CT ct) noexcept
      : df_(std::move(df)), ct_(std::move(ct)) {}

  inline void match(char const* const in, int const remaining,
                    OnMatcher const& onMatcher) const {
    match(reinterpret_cast<byte const*>(in), remaining, onMatcher);
  }

  inline void match(byte const* const in, int const remaining,
                    OnMatcher const& onMatcher) const {
    if (df_.contains(in)) {
      ct_.exactMatching(in, remaining, onMatcher);
    }
  }

  CT const& ct() const noexcept { return ct_; }
};
}  // namespace dfc

#endif
