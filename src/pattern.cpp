#include <cstring>

#include "pattern.hpp"

namespace dfc {
bool Pattern::operator==(Pattern const& other) const {
  return size() == other.size() &&
         std::memcmp(data(), other.data(), size()) == 0;
}

bool Pattern::operator<(Pattern const& other) const {
  if (size() != other.size()) {
    return size() < other.size();
  }

  return std::memcmp(data(), other.data(), size()) < 0;
}

bool Pattern::operator>(Pattern const& other) const {
  if (size() != other.size()) {
    return size() > other.size();
  }

  return std::memcmp(data(), other.data(), size()) > 0;
}
}  // namespace dfc
