#ifndef DFC_DF_INITIALIZER_HPP
#define DFC_DF_INITIALIZER_HPP

#include <cstdint>
#include <limits>

#include "df.hpp"
#include "pattern.hpp"

namespace dfc {

template <typename Indexer>
class DfInitializer {
  using Filter = typename std::array<
      byte, ((std::numeric_limits<typename Indexer::RetType>::max() + 1) >> 3)>;

  Filter filter_;
  int const minLengthPattern_, maxLengthPattern_;
  Indexer indexer_;
  Segmenter<typename Indexer::Segment> segmenter_;
  DfMasker<typename Indexer::Segment> masker_;

 public:
  explicit DfInitializer(int minLengthPattern, int maxLengthPattern) noexcept
      : filter_({}),
        minLengthPattern_(minLengthPattern),
        maxLengthPattern_(maxLengthPattern) {}

  // TODO: If case insensitive, create all permutations of segment
  // TODO: If shorter than segment, extend with all permutation
  void addPattern(const RawPattern& pat) noexcept {
    if (pat.size() >= minLengthPattern_ && pat.size() <= maxLengthPattern_) {
      const auto segment = segmenter_.segment(pat.data());
      const auto index = indexer_.index(segment);
      const auto mask = masker_.mask(segment);

      filter_[index] |= mask;
    }
  }

  const Filter& filter() const noexcept { return filter_; }

  DirectFilter<Indexer> df() const noexcept {
    return DirectFilter<Indexer>(filter_);
  }
};
}  // namespace dfc

#endif
