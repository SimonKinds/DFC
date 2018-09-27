#ifndef DFC_MATCHER_HPP
#define DFC_MATCHER_HPP

#include <array>
#include <limits>

#include "immutable-pattern.hpp"
#include "input-view.hpp"

namespace dfc {
class Matcher {
 public:
  inline bool matches(InputView const &input,
                      ImmutablePattern const &pattern) const noexcept {
    if (pattern.size() <= input.size()) {
      return matchesWithoutBounds(input.data(), pattern);
    }

    return false;
  }

  inline static bool compareWithCaseSensitivity(byte const *const first,
                                                byte const *const second,
                                                int const size) noexcept {
    /*
     * memcmp is faster in all cases (up to 16x for 1024 characters) instead of
     * using a loop
     */
    return std::memcmp(first, second, size) == 0;
  }

  inline static bool compareWithoutCaseSensitivity(byte const *const first,
                                                   byte const *const second,
                                                   int const size) noexcept {
    bool doesMatch = true;
    int i = 0;
    while (i < size && doesMatch) {
      doesMatch = (first[i] | 32) == (second[i] | 32);
      ++i;
    }

    return doesMatch;
  }

 private:
  inline bool matchesWithoutBounds(byte const *const in,
                                   ImmutablePattern const &pattern) const
      noexcept {
    if (pattern.caseSensitivity() == Pattern::CaseSensitivity::CaseSensitive) {
      return compareWithCaseSensitivity(in, pattern.data(), pattern.size());
    } else {
      return compareWithoutCaseSensitivity(in, pattern.data(), pattern.size());
    }
  }
};
}  // namespace dfc

#endif
