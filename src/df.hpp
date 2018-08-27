#ifndef DFC_DF_HPP
#define DFC_DF_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <limits>
#include <memory>
#include <vector>

#include "df-masker.hpp"
#include "indexer.hpp"
#include "input-view.hpp"
#include "pattern-range.hpp"
#include "pattern.hpp"
#include "segment-extender.hpp"
#include "segmenter.hpp"

namespace dfc {

template <typename SegmentType_, SegmentType_ Hash = 1,
          typename IndexType = SegmentType_>
class DirectFilter {
  static_assert(std::is_integral<SegmentType_>::value,
                "SegmentType must be integral");
  static_assert(std::is_integral<IndexType>::value,
                "IndexType must be integral");

 public:
  using SegmentType = SegmentType_;

 private:
  using Filter =
      typename std::array<byte,
                          ((std::numeric_limits<IndexType>::max() + 1) >> 3)>;

  Filter filter_{};

  Segmenter<SegmentType> const segmenter_{};
  DirectFilterIndexer<SegmentType, Hash, IndexType> const indexer_{};
  DirectFilterMasker<SegmentType> const masker_{};

 public:
  Filter const &filter() const noexcept { return filter_; }

  inline int indexByteCount() const noexcept { return sizeof(SegmentType); }

  inline bool contains(char const *const in) const noexcept {
    return contains(reinterpret_cast<byte const *>(in));
  }

  inline bool contains(byte const *const in) const noexcept {
    auto const segment = segmenter_.segment(in);
    auto const index = indexer_.index(segment);
    auto const mask = masker_.mask(segment);

    return filter_[index] & mask;
  }

  void addPattern(Pattern const &pattern) {
    if (shouldExtendSegment(pattern)) {
      auto permutations = extendSegment(pattern);
      for (auto const &permutation : permutations) {
        addPattern(permutation.data(), pattern.caseSensitive());
      }
    } else {
      addPattern(pattern.data(), pattern.caseSensitive());
    }
  }

 private:
  bool shouldExtendSegment(Pattern const &pattern) const noexcept {
    return pattern.size() < static_cast<int>(sizeof(SegmentType));
  }

  auto extendSegment(Pattern const &pattern) const {
    SegmentExtender<SegmentType> extender;

    return extender.extend(pattern);
  }

  void addPattern(byte const *const pattern, bool caseSensitive) noexcept {
    if (caseSensitive) {
      addPatternWithoutPermutations(pattern);
    } else {
      addPatternWithPermutations(pattern);
    }
  }

  void addPatternWithoutPermutations(byte const *const pattern) noexcept {
    addSegment(segmenter_.segment(pattern));
  }

  void addPatternWithPermutations(byte const *const pattern) noexcept {
    for (auto const segment : segmenter_.permutations(pattern)) {
      addSegment(segment);
    }
  }

  void addSegment(SegmentType const segment) noexcept {
    auto const index = indexer_.index(segment);
    auto const mask = masker_.mask(segment);

    filter_[index] |= mask;
  }
};
}  // namespace dfc

#endif
