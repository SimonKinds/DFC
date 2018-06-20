#include <type_traits>
#include "byte.hpp"

namespace dfc {
template <typename RetType, RetType Hash>
class DfIndexer {
 public:
  constexpr inline RetType index(const char* val) const noexcept {
    return index(reinterpret_cast<const byte*>(val));
  }
  constexpr inline RetType index(const byte* val) const noexcept {
    return (*reinterpret_cast<const RetType*>(val) * Hash) >> 3;
  }
};

using TwoByteDfIndexer = DfIndexer<uint16_t, 1>;
using FourByteHashDfIndexer = DfIndexer<uint16_t, 26693>;

template <typename RetType, RetType Hash, RetType Mask>
class CtIndexer {
 public:
  constexpr inline RetType index(const char* val) const noexcept {
    return index(reinterpret_cast<const byte*>(val));
  }

  constexpr inline RetType index(const byte* val) const noexcept {
    return (*reinterpret_cast<const RetType*>(val) * Hash) & Mask;
  }
};
}  // namespace dfc
