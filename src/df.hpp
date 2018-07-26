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
    auto const characterValues = std::numeric_limits<uint8_t>::max() + 1;
    auto const permutationCount =
        std::pow(characterValues, segmentSize() - pattern.size());

    SegmentPermutation permutation;
    std::memcpy(permutation.data(), pattern.data(), pattern.size());
    std::vector<SegmentPermutation> permutations(permutationCount, permutation);

    extendSegment(std::begin(permutations), pattern.size(), segmentSize());

    return permutations;
  }

  void extendSegment(typename std::vector<SegmentPermutation>::iterator it,
                     int const index, int const maxSize) const {
    if (index == maxSize) {
      return;
    }

    auto const characterValues = std::numeric_limits<uint8_t>::max() + 1;
    auto const permutationsOnNextLevel =
        std::pow(characterValues, maxSize - (index + 1)) - 1;

    for (int i = 0; i < characterValues; ++i) {
      // extend current pattern with the value of i
      it->at(index) = i;

      // pass new pattern to extendSegment
      extendSegment(it, index + 1, maxSize);

      it += permutationsOnNextLevel + 1;
    }
  }

  int segmentSize() const noexcept { return sizeof(SegmentType); }
};
}  // namespace dfc

#endif
