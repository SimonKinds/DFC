#ifndef DFC_MATCHER_HPP
#define DFC_MATCHER_HPP

#include "byte.hpp"
#include "pattern.hpp"

namespace dfc {
class Matcher {
 public:
  virtual ~Matcher() noexcept = default;

  inline bool matches(char const* const in, int const remaining,
                      Pattern const& pattern) const noexcept {
    return matches(reinterpret_cast<byte const*>(in), remaining, pattern);
  }

  // TODO: add case sensitivity
  inline bool matches(byte const* const in, int const remaining,
                      Pattern const& pattern) const noexcept {
    if (pattern.size() <= remaining) {
      return matchesWithoutBounds(in, pattern);
    }

    return false;
  }

 protected:
  virtual bool matchesWithoutBounds(byte const* const in,
                                    Pattern const& pattern) const noexcept = 0;
};

class MemcmpMatcher : public Matcher {
 protected:
  bool matchesWithoutBounds(byte const* const in, Pattern const& pattern) const
      noexcept final {
    return std::memcmp(in, pattern.data(), pattern.size()) == 0;
  }
};

class LoopMatcher : public Matcher {
 protected:
  bool matchesWithoutBounds(byte const* const in, Pattern const& pattern) const
      noexcept final {
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

#endif
