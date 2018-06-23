#ifndef DFC_INDEXER_HPP
#define DFC_INDEXER_HPP

#include <type_traits>
#include "byte.hpp"

namespace dfc {
template <typename SegmentType, SegmentType Hash, typename RetType>
class DirectFilterIndexer {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");
  static_assert(std::is_integral<RetType>::value, "RetType must be integral");

 public:
  constexpr inline RetType index(const SegmentType segment) const noexcept {
    return (segment * Hash) >> 3;
  }
};

template <typename SegmentType, SegmentType Hash, SegmentType Mask>
class CompactTableIndexer {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");

 public:
  constexpr inline SegmentType index(const SegmentType segment) const noexcept {
    return (segment * Hash) & Mask;
  }
};
}  // namespace dfc

#endif
