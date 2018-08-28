#ifndef DFC_DF_INTERFACE_HPP
#define DFC_DF_INTERFACE_HPP

#include "byte.hpp"

namespace dfc {
class Pattern;

class DirectFilterInterface {
 public:
  virtual ~DirectFilterInterface() = default;

  virtual int indexByteCount() const noexcept = 0;

  virtual bool contains(char const *const in) const noexcept = 0;
  virtual bool contains(byte const *const in) const noexcept = 0;

  virtual void addPattern(Pattern const &pattern) = 0;
};
}  // namespace dfc

#endif