#ifndef DFC_CT_INITIALIZER_HPP
#define DFC_CT_INITIALIZER_HPP

#include <array>
#include <limits>
#include <vector>

#include "byte.hpp"
#include "ct.hpp"
#include "indexer.hpp"
#include "on-matcher.hpp"
#include "pattern.hpp"
#include "segmenter.hpp"

namespace dfc {
template <typename SegmentType, SegmentType Hash, int Size>
class CompactTableInitializer {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");

  using Table = std::array<std::vector<CompactTableEntry<SegmentType>>, Size>;
  Table table_;

  CompactTableIndexer<SegmentType, Hash, Size - 1> const indexer_{};
  Segmenter<SegmentType> const segmenter_{};

 public:
  void addPattern(PidIndex const pidIndex, Pattern const& pattern) noexcept {
    auto const segment = segmenter_.segment(pattern.data());
    auto const bucketIndex = indexer_.index(segment);

    auto& bucket = table_[bucketIndex];

    bool found = false;
    for (auto& entry : bucket) {
      if (entry.segment == segment) {
        entry.pids.emplace_back(pidIndex);
        found = true;
        break;
      }
    }

    if (!found) {
      bucket.emplace_back(segment, pidIndex);
    }
  }

  template <typename SubOnMatcher>
  auto ct(std::shared_ptr<std::vector<Pattern> const> patterns) const {
    return CompactTable<SegmentType, Hash, Size, OnMatcher<SubOnMatcher>>(
        table_, std::move(patterns));
  }
};
}  // namespace dfc

#endif
