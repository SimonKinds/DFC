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

  void addPattern(Pattern const& pattern) {
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

  bool shouldExtendSegment(Pattern const& pattern) const noexcept {
    return pattern.size() < static_cast<int>(sizeof(SegmentType));
  }

 public:
  struct TooManyPermutationsException : public std::exception {
    char const* what() const throw() {
      return "The supplied pattern requires a segment extension of more than 1 "
             "byte";
    }
  };

 private:
  using SegmentPermutation = std::array<byte, sizeof(SegmentType)>;
  std::vector<SegmentPermutation> extendSegment(Pattern const& pattern) const {
    // if we allow 2 bytes, we're already at 16k permutations
    auto const allowedBytesToExtend = 1;

    if (sizeof(SegmentType) - pattern.size() > allowedBytesToExtend) {
      throw TooManyPermutationsException();
    }

    auto const characterValueCount = std::numeric_limits<uint8_t>::max() + 1;
    auto const permutationCount = characterValueCount;

    SegmentPermutation permutation;
    std::memcpy(permutation.data(), pattern.data(), pattern.size());
    std::vector<SegmentPermutation> permutations(permutationCount, permutation);

    auto it = std::begin(permutations);
    for (int val = 0; val < characterValueCount; ++val) {
      it->at(allowedBytesToExtend) = val;
      ++it;
    }

    return permutations;
  }
};
}  // namespace dfc

#endif
