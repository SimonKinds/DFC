#ifndef DFC_SEGMENTER_HPP
#define DFC_SEGMENTER_HPP

#include <array>

#include "byte.hpp"
#include "pattern.hpp"

namespace dfc {
template <typename SegmentType>
struct Segmenter {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");

  auto permutations(Pattern const& pattern) const noexcept {
    return permutations(pattern.data());
  }

  auto permutations(byte const* const in) const noexcept {
    int const segmentSize = sizeof(SegmentType);
    int const permutationCount = 2 << (segmentSize - 1);
    std::array<SegmentType, permutationCount> permutations{};

    for (int permutationIndex = 0; permutationIndex < permutationCount;
         ++permutationIndex) {
      uint64_t permutation = 0;

      for (int characterIndex = 0; characterIndex < segmentSize;
           ++characterIndex) {
        permutation <<= 8;
        permutation |= (((permutationIndex >> characterIndex) & 1) != 0)
                           ? std::toupper(in[characterIndex])
                           : in[characterIndex];
      }

      permutations[permutationIndex] = static_cast<SegmentType>(permutation);
    }

    return permutations;
  }

  inline SegmentType segment(Pattern const& pattern) const noexcept {
    return segment(pattern.data());
  }

  inline SegmentType segment(byte const* const in) const noexcept {
    return *reinterpret_cast<SegmentType const*>(in);
  }
};
}  // namespace dfc

#endif
