#ifndef DFC_DF_INITIALIZER_HPP
#define DFC_DF_INITIALIZER_HPP

#include <cstdint>
#include <limits>

#include "df.hpp"
#include "indexer.hpp"
#include "pattern.hpp"

namespace dfc {

template <typename SegmentType, SegmentType Hash = 1,
          typename IndexType = SegmentType>
class DfInitializer {
  using Filter =
      typename std::array<byte,
                          ((std::numeric_limits<IndexType>::max() + 1) >> 3)>;
  using Indexer = DfIndexer<SegmentType, Hash, IndexType>;

  Filter filter_;
  int const minLengthPattern_, maxLengthPattern_;
  Indexer const indexer_{};
  Segmenter<SegmentType> const segmenter_{};
  DfMasker<SegmentType> const masker_{};

 public:
  explicit DfInitializer(int minLengthPattern, int maxLengthPattern) noexcept
      : filter_({}),
        minLengthPattern_(minLengthPattern),
        maxLengthPattern_(maxLengthPattern) {}

  // TODO: If case insensitive, create all permutations of segment
  // TODO: If shorter than segment, extend with all permutation
  void addPattern(const RawPattern& pat) noexcept {
    if (pat.size() >= minLengthPattern_ && pat.size() <= maxLengthPattern_) {
      auto const segment = segmenter_.segment(pat.data());
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
