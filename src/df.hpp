#ifndef DFC_DF_INITIALIZER_HPP
#define DFC_DF_INITIALIZER_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <limits>
#include <memory>
#include <vector>

#include "df-masker.hpp"
#include "indexer.hpp"
#include "pattern-range.hpp"
#include "pattern.hpp"
#include "segmenter.hpp"

namespace dfc {

template <typename PatternRange, typename SegmentType, SegmentType Hash = 1,
          typename IndexType = SegmentType>
class DirectFilter {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");
  static_assert(std::is_integral<IndexType>::value,
                "IndexType must be integral");

  using Filter =
      typename std::array<byte,
                          ((std::numeric_limits<IndexType>::max() + 1) >> 3)>;

  Filter filter_{};

  PatternRange const patternRange_{};

  Segmenter<SegmentType> const segmenter_{};
  DirectFilterIndexer<SegmentType, Hash, IndexType> const indexer_{};
  DirectFilterMasker<SegmentType> const masker_{};

 public:
  Filter const& filter() const noexcept { return filter_; }

  inline bool contains(char const* const in) const noexcept {
    return contains(reinterpret_cast<byte const*>(in));
  }

  inline bool contains(byte const* const in) const noexcept {
    auto const segment = segmenter_.segment(in);
    auto const index = indexer_.index(segment);
    auto const mask = masker_.mask(segment);

    return filter_[index] & mask;
  }

  void addPattern(Pattern const& pattern) noexcept {
    if (patternRange_.includes(pattern)) {
      if (shouldExtendSegment(pattern)) {
        auto permutations = extendSegment(pattern);
        for (auto const& permutation : permutations) {
          addPattern(permutation.data(), pattern.caseSensitive());
        }
      } else {
        addPattern(pattern.data(), pattern.caseSensitive());
      }
    }
  }

 private:
  bool shouldExtendSegment(Pattern const& pattern) const noexcept {
    return pattern.size() < static_cast<int>(sizeof(SegmentType));
  }

 private:
  void addPattern(byte const* const pattern, bool caseSensitive) noexcept {
    if (caseSensitive) {
      addPatternWithoutPermutations(pattern);
    } else {
      addPatternWithPermutations(pattern);
    }
  }

  void addPatternWithoutPermutations(byte const* const pattern) noexcept {
    addSegment(segmenter_.segment(pattern));
  }

  void addPatternWithPermutations(byte const* const pattern) noexcept {
    for (auto const segment : segmenter_.permutations(pattern)) {
      addSegment(segment);
    }
  }

  void addSegment(SegmentType const segment) noexcept {
    auto const index = indexer_.index(segment);
    auto const mask = masker_.mask(segment);

    filter_[index] |= mask;
  }

  using SegmentPermutation = std::array<byte, sizeof(SegmentType)>;
  std::vector<SegmentPermutation> extendSegment(Pattern const& pattern) const {
    SegmentPermutation segment;
    std::memcpy(segment.data(), pattern.data(), pattern.size());

    return extendSegment(segment, pattern.size(), segmentSize());
  }

  std::vector<SegmentPermutation> extendSegment(
      SegmentPermutation const& prefix, int const index,
      int const maxSize) const {
    auto const characterValues = std::numeric_limits<uint8_t>::max() + 1;
    auto const remainingPermutations =
        std::pow(characterValues, maxSize - index) - 1;

    if (remainingPermutations == 0) {
      return {prefix};
    }

    std::vector<SegmentPermutation> permutations;
    permutations.reserve(remainingPermutations);

    for (int i = 0; i < characterValues; ++i) {
      // extend current pattern with the value of i
      SegmentPermutation segment;
      std::memcpy(segment.data(), prefix.data(), index);
      segment[index] = i;

      // pass new pattern to extendSegment
      auto newPermutations = extendSegment(segment, index + 1, maxSize);

      // move the returned values from into the permutations vector
      std::move(std::begin(newPermutations), std::end(newPermutations),
                std::back_inserter(permutations));
    }

    return permutations;
  }

  int segmentSize() const noexcept { return sizeof(SegmentType); }
};
}  // namespace dfc

#endif
