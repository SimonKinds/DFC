#ifndef DFC_DF_MASK_HPP
#define DFC_DF_MASK_HPP

#include "byte.hpp"

namespace dfc {
template <typename Segment>
class DirectFilterMasker {
 public:
  inline byte mask(const Segment segment) const noexcept {
    return (segment & 0x7) << 1;
  }
};
}  // namespace dfc

#endif
