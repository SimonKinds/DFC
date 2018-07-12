#ifndef DFC_DF_INITIALIZER_HPP
#define DFC_DF_INITIALIZER_HPP

#include <cstdint>
#include <limits>

#include "df.hpp"
#include "indexer.hpp"
#include "pattern-range.hpp"
#include "pattern.hpp"

namespace dfc {

template <typename PatternRange, typename SegmentType, SegmentType Hash = 1,
          typename IndexType = SegmentType>
class DirectFilterInitializer {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");
  static_assert(std::is_integral<IndexType>::value,
                "IndexType must be integral");

  using Filter =
      typename std::array<byte,
                          ((std::numeric_limits<IndexType>::max() + 1) >> 3)>;
  using Indexer = DirectFilterIndexer<SegmentType, Hash, IndexType>;

  Filter filter_{};

  PatternRange const patternRange_{};

  Indexer const indexer_{};
  Segmenter<SegmentType> const segmenter_{};
  DirectFilterMasker<SegmentType> const masker_{};

 public:
  // TODO: If case insensitive, create all permutations of segment
  // TODO: If shorter than segment, extend with all permutation
  void addPattern(RawPattern const& pattern) noexcept {
    if (patternRange_.includes(pattern)) {
      auto const segment = segmenter_.segment(pattern);
      auto const index = indexer_.index(segment);
      auto const mask = masker_.mask(segment);

      filter_[index] |= mask;
    }
  }

  Filter const& filter() const noexcept { return filter_; }

  auto df() const noexcept {
    return DirectFilter<SegmentType, Hash, IndexType>(filter_);
  }
};
}  // namespace dfc

#endif
