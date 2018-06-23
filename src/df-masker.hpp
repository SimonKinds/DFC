#ifndef DFC_DF_MASK_HPP
#define DFC_DF_MASK_HPP

#include <type_traits>

#include "byte.hpp"

namespace dfc {
template <typename SegmentType>
class DirectFilterMasker {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");

 public:
  inline byte mask(const SegmentType segment) const noexcept {
    return (segment & 0x7) << 1;
  }
};
}  // namespace dfc

#endif
