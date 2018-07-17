#ifndef DFC_CT_INITIALIZER_HPP
#define DFC_CT_INITIALIZER_HPP

#include <array>
#include <limits>
#include <vector>

#include "byte.hpp"
#include "ct.hpp"
#include "pattern-range.hpp"

namespace dfc {
template <typename PatternRange, typename SegmentType, SegmentType Hash,
          int Size>
class CompactTableInitializer {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");
  static_assert(
      PatternRange::startInclusive == sizeof(SegmentType),
      "SegmentType must be equal in size to the smallest pattern length");

  using Table = std::array<std::vector<CompactTableEntry<SegmentType>>, Size>;
  Table table_;

  CompactTableIndexer<SegmentType, Hash, Size - 1> const indexer_{};
  Segmenter<SegmentType> const segmenter_{};

  PatternRange const patternRange_{};

 public:
  void addPattern(PidIndex const pidIndex, Pattern const& pattern) noexcept {
    if (patternRange_.includes(pattern)) {
      if (pattern.caseSensitive()) {
        addPatternWithoutPermutations(pidIndex, pattern);
      } else {
        addPatternWithPermutations(pidIndex, pattern);
      }
    }
  }

  template <typename OnMatcher>
  auto ct(std::shared_ptr<OnMatcher> onMatcher,
          std::shared_ptr<std::vector<ImmutablePattern> const> patterns) const {
    static_assert(std::is_base_of<dfc::OnMatcher, OnMatcher>::value,
                  "OnMatcher template parameter is not an OnMatcher");

    return CompactTable<SegmentType, Hash, Size, OnMatcher>(
        table_, std::move(onMatcher), std::move(patterns));
  }

 private:
  void addPatternWithoutPermutations(PidIndex pidIndex,
                                     Pattern const& pattern) {
    addPatternForSegment(pidIndex, segmenter_.segment(pattern));
  }

  void addPatternWithPermutations(PidIndex pidIndex,
                                  Pattern const& pattern) noexcept {
    for (auto const segment : segmenter_.permutations(pattern)) {
      addPatternForSegment(pidIndex, segment);
    }
  }

  void addPatternForSegment(PidIndex pidIndex, SegmentType segment) noexcept {
    auto const bucketIndex = indexer_.index(segment);

    auto& bucket = table_[bucketIndex];

    auto entry = std::begin(bucket);
    auto const end = std::cend(bucket);
    bool found = false;
    while (entry != end && !found) {
      if (entry->segment == segment) {
        entry->pids.emplace_back(pidIndex);
        found = true;
      }
      ++entry;
    }

    if (!found) {
      bucket.emplace_back(segment, pidIndex);
    }
  }
};
}  // namespace dfc

#endif
