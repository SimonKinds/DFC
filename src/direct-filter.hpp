#ifndef DFC_DF_HPP
#define DFC_DF_HPP

#include "byte.hpp"

namespace dfc {
class Pattern;

class DirectFilter {
 public:
  virtual ~DirectFilter() = default;

  virtual int indexByteCount() const noexcept = 0;

  virtual bool contains(char const *const in) const noexcept = 0;
  virtual bool contains(byte const *const in) const noexcept = 0;

  virtual void addPattern(Pattern const &pattern) = 0;
};
}  // namespace dfc

#endif
