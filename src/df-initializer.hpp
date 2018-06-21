#include <cstdint>
#include <limits>
#include <type_traits>

#include "pattern.hpp"

namespace dfc {

template <typename Indexer, typename Segmenter>
class DfInitializer {
  using Df = std::shared_ptr<byte[]>;
  Df df_;
  int const minLengthPattern_, maxLengthPattern_;
  Indexer indexer_;
  Segmenter segmenter_;

 public:
  explicit DfInitializer(int minLengthPattern, int maxLengthPattern) noexcept
      : df_(std::make_unique<byte[]>(dfSizeBytes())),
        minLengthPattern_(minLengthPattern),
        maxLengthPattern_(maxLengthPattern) {
    static_assert(std::is_same<typename Indexer::Segment,
                               typename Segmenter::Type>::value,
                  "The segmenter does not return the type used by the indexer");
  }

  void addPattern(const RawPattern& pat) noexcept {
    if (pat.size() >= minLengthPattern_ && pat.size() <= maxLengthPattern_) {
      const auto segment = segmenter_.segment(pat.data());
      const auto index = indexer_.index(segment);
      const auto mask = (segment & 0x7) << 1;

      df_[index] |= mask;
    }
  }

  Df df() const noexcept { return df_; }

  int dfSizeBytes() const noexcept {
    return (std::numeric_limits<typename Indexer::RetType>::max() + 1) >> 3;
  }
};
}  // namespace dfc
