#ifndef DFC_DF_HPP
#define DFC_DF_HPP

#include <array>
#include <limits>

#include "byte.hpp"
#include "df-masker.hpp"
#include "indexer.hpp"
#include "segmenter.hpp"

namespace dfc {
template <typename SegmentType, SegmentType Hash, typename IndexType>
class DirectFilter {
  using Filter =
      typename std::array<byte,
                          ((std::numeric_limits<IndexType>::max() + 1) >> 3)>;

  Filter const filter_;
  DirectFilterIndexer<SegmentType, Hash, IndexType> const indexer_{};
  Segmenter<SegmentType> const segmenter_{};
  DirectFilterMasker<SegmentType> const masker_{};

 public:
  explicit DirectFilter(Filter filter) : filter_(std::move(filter)) {}

  inline bool isSet(const byte* in) const noexcept {
    auto const segment = segmenter_.segment(in);
    auto const index = indexer_.index(segment);
    auto const mask = masker_.mask(segment);

    return filter_[index] & mask;
  }

  Filter const& filter() const noexcept { return filter_; }
};
}  // namespace dfc

#endif
