#ifndef DFC_PATTERN_HPP
#define DFC_PATTERN_HPP

#include "byte.hpp"

namespace dfc {
class Pattern {
 public:
  enum class CaseSensitivity : bool { CaseSensitive, CaseInsensitive };

  virtual ~Pattern() = default;

  virtual byte const* data() const noexcept = 0;
  virtual int size() const noexcept = 0;
  virtual CaseSensitivity caseSensitivity() const noexcept = 0;

  virtual bool operator==(Pattern const& other) const;
  virtual bool operator<(Pattern const& other) const;
  virtual bool operator>(Pattern const& other) const;
};
}  // namespace dfc

#endif
