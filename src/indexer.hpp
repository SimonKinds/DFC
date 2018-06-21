#include <type_traits>
#include "byte.hpp"

namespace dfc {
template <typename Segment_, Segment_ Hash, typename RetType_ = Segment_>
class DfIndexer {
 public:
  using Segment = Segment_;
  using RetType = RetType_;
  constexpr inline RetType index(const Segment segment) const noexcept {
    return (segment * Hash) >> 3;
  }
};

using TwoByteDfIndexer = DfIndexer<uint16_t, 1>;
using FourByteHashDfIndexer = DfIndexer<uint32_t, 26693, uint16_t>;

template <typename Segment, Segment Hash, Segment Mask,
          typename RetType = Segment>
class CtIndexer {
 public:
  constexpr inline RetType index(const Segment segment) const noexcept {
    return (segment * Hash) & Mask;
  }
};
}  // namespace dfc
