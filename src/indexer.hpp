#include <type_traits>
#include "byte.hpp"

namespace dfc {
template <typename RetType, RetType Hash = 1>
class DfIndexer {
 public:
  constexpr inline RetType index(const char* val) const noexcept {
    return index(reinterpret_cast<const byte*>(val));
  }
  constexpr inline RetType index(const byte* val) const noexcept {
    return (*reinterpret_cast<const RetType*>(val) * Hash) >> 3;
  }
};

using TwoByteDfIndexer = DfIndexer<uint16_t>;
using FourByteHashDfIndexer = DfIndexer<uint16_t, 26693>;
}  // namespace dfc
