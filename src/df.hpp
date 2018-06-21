#ifndef DFC_DF_HPP
#define DFC_DF_HPP

#include <array>
#include <limits>

#include "byte.hpp"
#include "df-masker.hpp"
#include "indexer.hpp"
#include "segmenter.hpp"

namespace dfc {
template <typename Indexer>
class DirectFilter {
  using Filter = typename std::array<
      byte, ((std::numeric_limits<typename Indexer::RetType>::max() + 1) >> 3)>;

  const Filter filter_;
  Indexer indexer_;
  Segmenter<typename Indexer::Segment> segmenter_;
  DfMasker<typename Indexer::Segment> masker_;

 public:
  explicit DirectFilter(Filter filter) : filter_(std::move(filter)) {}

  inline bool isSet(const byte* in) const noexcept {
    const auto segment = segmenter_.segment(in);
    const auto index = indexer_.index(segment);
    const auto mask = masker_.mask(segment);

    return filter_[index] & mask;
  }

  const Filter& filter() const noexcept { return filter_; }
};
}  // namespace dfc

#endif
