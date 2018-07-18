#ifndef DFC_MATCHER_HPP
#define DFC_MATCHER_HPP

#include "byte.hpp"
#include "immutable-pattern.hpp"

namespace dfc {
class Matcher {
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
    // memcmp is faster in all cases (up to 16x for 1024 characters) when
    // comparing byte for byte
    return std::memcmp(in, pattern.data(), pattern.size()) == 0;
  }

  // TODO: try to improve
  inline bool matchesCaseInsensitive(byte const* const in,
                                     ImmutablePattern const& pattern) const
      noexcept {
    auto const data = pattern.data();
    bool matches = true;
    int i = 0;
    while (i < pattern.size() && matches) {
      matches = std::tolower(data[i]) == std::tolower(in[i]);
      ++i;
    }

    return matches;
  }
};
}  // namespace dfc

#endif
