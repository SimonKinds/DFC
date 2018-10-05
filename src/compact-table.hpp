#ifndef DFC_CT_HPP
#define DFC_CT_HPP

#include <array>
#include <vector>

#include "byte.hpp"
#include "indexer.hpp"
#include "matcher.hpp"
#include "on-matcher.hpp"
#include "pattern-range.hpp"
#include "pid.hpp"
#include "segmenter.hpp"

namespace dfc {
template <typename SegmentType>
struct CompactTableEntry {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");

  CompactTableEntry(SegmentType const segment, PidIndex const pid)
      : segment(segment), pids({pid}) {}

  SegmentType const segment;
  std::vector<PidIndex> pids;
};

template <typename SegmentType_, SegmentType_ Hash, int Size>
class CompactTable {
  static_assert(std::is_integral<SegmentType_>::value,
                "SegmentType must be integral");

 public:
  using SegmentType = SegmentType_;

 private:
  using Entry = CompactTableEntry<SegmentType>;
  using Bucket = std::vector<Entry>;
  using Table = std::array<Bucket, Size>;

  CompactTableIndexer<SegmentType, Hash, Size - 1> const indexer_{};
  Segmenter<SegmentType> const segmenter_{};

  Matcher const matcher_{};

  std::vector<ImmutablePattern> patterns_{};

  /*
   * do not explicitly initialize as gcc compile times goes through the roof for
   * large arrays. Since the buckets (std::vector at the time of writing) have a
   * non-trivial constructor, the entries of the array are ensured to be
   * properly intialized
   */
  Table table_;

 public:
  inline int indexByteCount() const noexcept { return sizeof(SegmentType); }

  void findAllMatches(InputView const &input,
                      OnMatcher const &onMatcher) const {
    auto const &bucket = getBucket(input.data());

    findMatchesInBucket(bucket, input, onMatcher);
  }

 private:
  Bucket const &getBucket(byte const *const in) const noexcept {
    auto const segment = segmenter_.segment(in);
    auto const index = indexer_.index(segment);

    return table_[index];
  }

  void findMatchesInBucket(Bucket const &bucket, InputView const &input,
                           OnMatcher const &onMatcher) const {
    auto const segment = segmenter_.segment(input.data());

    auto entry = std::cbegin(bucket);
    auto const end = std::cend(bucket);
    bool found = false;
    while (entry != end && !found) {
      if (entry->segment == segment) {
        found = true;

        findMatchesInEntry(*entry, input, onMatcher);
      }

      ++entry;
    }
  }

  __attribute_noinline__ void findMatchesInEntry(
      Entry const &entry, InputView const &input,
      OnMatcher const &onMatcher) const {
    for (auto const pidIndex : entry.pids) {
      auto const &pattern = patterns_[pidIndex.value()];

      if (matcher_.matches(input, pattern)) {
        onMatcher.onMatch(pattern);
      }
    }
  }

 public:
  void addPattern(ImmutablePattern const &pattern) noexcept {
    PidIndex const index{static_cast<int>(patterns_.size())};

    if (pattern.caseSensitivity() == Pattern::CaseSensitivity::CaseSensitive) {
      addPatternToTableWithoutPermutations(index, pattern);
    } else {
      addPatternToTableWithPermutations(index, pattern);
    }

    patterns_.emplace_back(pattern);
  }

 private:
  void addPatternToTableWithoutPermutations(PidIndex pidIndex,
                                            Pattern const &pattern) noexcept {
    addPatternToTableForSegment(pidIndex, segmenter_.segment(pattern));
  }

  void addPatternToTableWithPermutations(PidIndex pidIndex,
                                         Pattern const &pattern) noexcept {
    for (auto const segment : segmenter_.permutations(pattern)) {
      addPatternToTableForSegment(pidIndex, segment);
    }
  }

  void addPatternToTableForSegment(PidIndex pidIndex,
                                   SegmentType segment) noexcept {
    auto const bucketIndex = indexer_.index(segment);

    auto &bucket = table_[bucketIndex];

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
