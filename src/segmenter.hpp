#ifndef DFC_SEGMENTER_HPP
#define DFC_SEGMENTER_HPP

#include <type_traits>
#include "byte.hpp"

namespace dfc {
template <typename SegmentType>
struct Segmenter {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");

  inline SegmentType segment(const byte* in) const noexcept {
    return *reinterpret_cast<const SegmentType*>(in);
  }
};
}  // namespace dfc

#endif
