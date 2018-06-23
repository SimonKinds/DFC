#ifndef DFC_INDEXER_HPP
#define DFC_INDEXER_HPP

#include <type_traits>
#include "byte.hpp"

namespace dfc {
template <typename Segment, Segment Hash, typename RetType>
class DirectFilterIndexer {
 public:
  constexpr inline RetType index(const Segment segment) const noexcept {
    return (segment * Hash) >> 3;
  }
};

template <typename Segment, Segment Hash, Segment Mask>
class CompactTableIndexer {
 public:
  constexpr inline Segment index(const Segment segment) const noexcept {
    return (segment * Hash) & Mask;
  }
};
}  // namespace dfc

#endif
