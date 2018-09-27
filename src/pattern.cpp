#include <cstring>

#include "matcher.hpp"
#include "pattern.hpp"

namespace dfc {
bool Pattern::operator==(Pattern const& other) const {
  if (size() == other.size() && caseSensitivity() == other.caseSensitivity()) {
    if (caseSensitivity() == Pattern::CaseSensitivity::CaseSensitive) {
      return Matcher::compareWithCaseSensitivity(data(), other.data(), size());
    } else {
      return Matcher::compareWithoutCaseSensitivity(data(), other.data(),
                                                    size());
    }
  }

  return false;
}

bool Pattern::operator<(Pattern const& other) const {
  bool isSmaller = false;
  if (size() != other.size()) {
    isSmaller = size() < other.size();
  } else if (caseSensitivity() != other.caseSensitivity()) {
    isSmaller = caseSensitivity() < other.caseSensitivity();
  } else {
    isSmaller = std::memcmp(data(), other.data(), size()) < 0;
  }

  return isSmaller;
}

bool Pattern::operator>(Pattern const& other) const {
  bool isGreater = false;
  if (size() != other.size()) {
    isGreater = size() > other.size();
  } else if (caseSensitivity() != other.caseSensitivity()) {
    isGreater = caseSensitivity() > other.caseSensitivity();
  } else {
    isGreater = std::memcmp(data(), other.data(), size()) > 0;
  }

  return isGreater;
}  // namespace dfc
}  // namespace dfc
