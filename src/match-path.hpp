#ifndef DFC_MATCH_PATH_HPP
#define DFC_MATCH_PATH_HPP

#include <utility>
#include "byte.hpp"

namespace dfc {
template <typename DF, typename CT>
class MatchPath {
 private:
  DF const df_;
  CT const ct_;

 public:
  explicit MatchPath(DF df, CT ct) noexcept
      : df_(std::move(df)), ct_(std::move(ct)) {}

  inline void match(char const* const in, int const remaining) const {
    match(reinterpret_cast<byte const*>(in), remaining);
  }

  inline void match(byte const* const in, int const remaining) const {
    if (df_.isSet(in)) {
      ct_.exactMatching(in, remaining);
    }
  }

  CT const& ct() const noexcept { return ct_; }
};
}  // namespace dfc

#endif
