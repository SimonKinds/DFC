#include "byte.hpp"
#include "pattern.hpp"

namespace dfc {
template <typename T>
struct Matcher {
  inline bool matches(byte const* const in, int const remaining,
                      Pattern const& pattern) const noexcept {
    // TODO: add case sensitivity
    auto const& derived = static_cast<T const&>(*this);
    if (remaining >= pattern.size()) {
      return derived.matches(in, pattern);
    }

    return false;
  }
};

struct MemcmpMatcher : public Matcher<MemcmpMatcher> {
  inline bool matches(byte const* const in, Pattern const& pattern) const
      noexcept {
    return std::memcmp(in, pattern.data(), pattern.size()) == 0;
  }
};

struct LoopMatcher : public Matcher<LoopMatcher> {
  inline bool matches(byte const* const in, Pattern const& pattern) const
      noexcept {
    auto const data = pattern.data();
    bool matches = true;
    int i = 0;
    while (i < pattern.size() && matches) {
      matches = data[i] == in[i];
      ++i;
    }

    return matches;
  }
};

}  // namespace dfc
