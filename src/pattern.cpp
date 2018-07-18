#include "pattern.hpp"

namespace dfc {
bool Pattern::operator==(Pattern const& other) const {
  return size() == other.size() && memcmp(data(), other.data(), size()) == 0;
}

bool Pattern::operator<(Pattern const& other) const {
  if (size() != other.size()) {
    return size() < other.size();
  }

  return memcmp(data(), other.data(), size()) == -1;
}

bool Pattern::operator>(Pattern const& other) const {
  if (size() != other.size()) {
    return size() > other.size();
  }

  return memcmp(data(), other.data(), size()) == 1;
}
}  // namespace dfc
