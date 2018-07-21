#ifndef DFC_MATCHER_HPP
#define DFC_MATCHER_HPP

#include <array>
#include <limits>
#include "byte.hpp"
#include "immutable-pattern.hpp"

namespace dfc {
class Matcher {
  using XlatCaseType =
      std::array<uint8_t, std::numeric_limits<uint8_t>::max() + 1>;

  static constexpr XlatCaseType initializeXlatCase() {
    XlatCaseType xlatcase{};
    for (int i = 0; i < static_cast<int>(xlatcase.size()); ++i) {
      int lower = i;
      if (i >= 65 && i <= 90) {
        lower = i + 32;
      }
      xlatcase[i] = lower;
    }

    return xlatcase;
  }

  XlatCaseType const xlatcase = initializeXlatCase();

 public:
  inline bool matches(char const* const in, int const remaining,
                      ImmutablePattern const& pattern) const noexcept {
    return matches(reinterpret_cast<byte const*>(in), remaining, pattern);
  }

  inline bool matches(byte const* const in, int const remaining,
                      ImmutablePattern const& pattern) const noexcept {
    if (pattern.size() <= remaining) {
      return matchesWithoutBounds(in, pattern);
    }

    return false;
  }

 private:
  inline bool matchesWithoutBounds(byte const* const in,
                                   ImmutablePattern const& pattern) const
      noexcept {
    if (pattern.caseSensitive()) {
      return matchesCaseSensitive(in, pattern);
    } else {
      return matchesCaseInsensitive(in, pattern);
    }
  }

  inline bool matchesCaseSensitive(byte const* const in,
                                   ImmutablePattern const& pattern) const
      noexcept {
    /*
     * memcmp is faster in all cases (up to 16x for 1024 characters) instead of
     * using a loop
     */
    return std::memcmp(in, pattern.data(), pattern.size()) == 0;
  }

  inline bool matchesCaseInsensitive(byte const* const in,
                                     ImmutablePattern const& pattern) const
      noexcept {
    auto const patternData = pattern.data();

    bool doesMatch = true;
    int i = 0;
    while (i < pattern.size() && doesMatch) {
      doesMatch = xlatcase[patternData[i]] == xlatcase[in[i]];
      ++i;
    }

    return doesMatch;
  }
};
}  // namespace dfc

#endif
