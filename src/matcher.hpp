#ifndef DFC_MATCHER_HPP
#define DFC_MATCHER_HPP

#include <array>
#include <limits>
#include "byte.hpp"
#include "character-transformer.hpp"
#include "immutable-pattern.hpp"

namespace dfc {
class Matcher {
  CharacterTransformer charTransformer_;

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
      doesMatch = toLower(patternData[i]) == toLower(in[i]);
      ++i;
    }

    return doesMatch;
  }

  inline uint8_t toLower(uint8_t val) const noexcept {
    return charTransformer_.toLower(val);
  }
};
}  // namespace dfc

#endif
