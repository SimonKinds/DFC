#ifndef DFC_SEGMENT_EXTENDER_HPP
#define DFC_SEGMENT_EXTENDER_HPP

#include <array>
#include <cstring>
#include <limits>
#include <optional>
#include <vector>

#include "byte.hpp"
#include "pattern.hpp"

namespace dfc {
template <typename SegmentType>
class SegmentExtender {
  static int const segmentSize = sizeof(SegmentType);
  // if we allow 2 bytes, we're already at 16k permutations
  static int const allowedBytesToExtend = 1;

  static constexpr int permutationCount() noexcept {
    return characterValueCount();
  }

  static constexpr int characterValueCount() noexcept {
    return std::numeric_limits<uint8_t>::max() + 1;
  }

 public:
  using SegmentPermutation = std::array<byte, segmentSize>;
  using PermutationCollection =
      std::array<SegmentPermutation, permutationCount()>;

  struct TooManyPermutationsException : public std::exception {
    char const* what() const throw() {
      return "The supplied pattern requires a segment extension of more than 1 "
             "byte";
    }
  };

  PermutationCollection extend(Pattern const& pattern) const {
    if (!isAllowed(pattern)) {
      throw TooManyPermutationsException();
    }

    auto permutations = initializePermutationContainer(pattern);

    auto const extensionIndex = pattern.size();
    auto it = std::begin(permutations);
    for (int val = 0; val < characterValueCount(); ++val) {
      it->at(extensionIndex) = val;
      ++it;
    }

    return permutations;
  }

 private:
  bool isAllowed(Pattern const& pattern) const noexcept {
    return segmentSize - pattern.size() <= allowedBytesToExtend;
  }

  PermutationCollection initializePermutationContainer(
      Pattern const& pattern) const {
    SegmentPermutation permutation;
    std::memcpy(permutation.data(), pattern.data(), pattern.size());

    PermutationCollection permutations;
    std::fill(std::begin(permutations), std::end(permutations), permutation);

    return permutations;
  }
};
}  // namespace dfc

#endif
