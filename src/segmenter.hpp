#ifndef DFC_SEGMENTER_HPP
#define DFC_SEGMENTER_HPP

#include "byte.hpp"

namespace dfc {
template <typename Type>
struct Segmenter {
  inline Type segment(const byte* in) const noexcept {
    return *reinterpret_cast<const Type*>(in);
  }
};
}  // namespace dfc

#endif
