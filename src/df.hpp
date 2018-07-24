#ifndef DFC_DF_INITIALIZER_HPP
#define DFC_DF_INITIALIZER_HPP

#include <array>
#include <limits>

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
  inline bool contains(char const* const in) const noexcept {
    return contains(reinterpret_cast<byte const*>(in));
  }

  inline bool contains(byte const* const in) const noexcept {
    auto const segment = segmenter_.segment(in);
    auto const index = indexer_.index(segment);
    auto const mask = masker_.mask(segment);

    return filter_[index] & mask;
  }

  // TODO: If shorter than segment, extend with all permutation
  void addPattern(Pattern const& pattern) noexcept {
    if (patternRange_.includes(pattern)) {
      if (pattern.caseSensitive()) {
        addPatternWithoutPermutations(pattern);
      } else {
        addPatternWithPermutations(pattern);
      }
    }
  }

  Filter const& filter() const noexcept { return filter_; }

 private:
  void addPatternWithoutPermutations(Pattern const& pattern) noexcept {
    addSegment(segmenter_.segment(pattern));
  }

  void addPatternWithPermutations(Pattern const& pattern) noexcept {
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
