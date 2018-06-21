#include "byte.hpp"

namespace dfc {
template <int ByteCount>
struct Segmenter {};

template <>
struct Segmenter<1> {
  using Type = uint8_t;
  inline Type segment(const byte* in) const noexcept { return *in; }
};

template <>
struct Segmenter<2> {
  using Type = uint16_t;
  inline Type segment(const byte* in) const noexcept {
    return *reinterpret_cast<const uint16_t*>(in);
  }
};

template <>
struct Segmenter<4> {
  using Type = uint32_t;
  inline Type segment(const byte* in) const noexcept {
    return *reinterpret_cast<const uint32_t*>(in);
  }
};
}  // namespace dfc
