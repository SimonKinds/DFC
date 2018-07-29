#ifndef DFC_CT_INITIALIZER_HPP
#define DFC_CT_INITIALIZER_HPP

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

template <typename PatternRange, typename SegmentType, SegmentType Hash,
          int Size>
class CompactTable {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");
  static_assert(
      PatternRange::startInclusive == sizeof(SegmentType),
      "SegmentType must be equal in size to the smallest pattern length");

  using Entry = CompactTableEntry<SegmentType>;
  using Bucket = std::vector<Entry>;
  using Table = std::array<Bucket, Size>;

  PatternRange const patternRange_{};

  CompactTableIndexer<SegmentType, Hash, Size - 1> const indexer_{};
  Segmenter<SegmentType> const segmenter_{};

  Matcher const matcher_{};

  std::vector<ImmutablePattern> patterns_{};

  Table table_;

 public:
  void findAllMatches(char const* const in, int const remaining,
                      OnMatcher const& onMatcher) const noexcept {
    findAllMatches(reinterpret_cast<byte const*>(in), remaining, onMatcher);
  }

  void findAllMatches(byte const* const in, int const remaining,
                      OnMatcher const& onMatcher) const {
    auto const& bucket = getBucket(in);

    findMatchesInBucket(bucket, in, remaining, onMatcher);
  }

 private:
  Bucket const& getBucket(byte const* const in) const {
    auto const segment = segmenter_.segment(in);
    auto const index = indexer_.index(segment);

    return table_[index];
  }

  void findMatchesInBucket(Bucket const& bucket, byte const* const in,
                           int const remaining,
                           OnMatcher const& onMatcher) const {
    auto const segment = segmenter_.segment(in);

    auto entry = std::cbegin(bucket);
    auto const end = std::cend(bucket);
    bool found = false;
    while (entry != end && !found) {
      if (entry->segment == segment) {
        found = true;

        findMatchesInEntry(*entry, in, remaining, onMatcher);
      }

      ++entry;
    }
  }

  void findMatchesInEntry(Entry const& entry, byte const* const in,
                          int const remaining,
                          OnMatcher const& onMatcher) const {
    for (auto const pidIndex : entry.pids) {
      auto const& pattern = patterns_.at(pidIndex);

      if (matcher_.matches(in, remaining, pattern)) {
        onMatcher.onMatch(pattern);
      }
    }
  }

 public:
  void addPattern(ImmutablePattern const& pattern) noexcept {
    if (patternRange_.includes(pattern)) {
      PidIndex const index = patterns_.size();

      if (pattern.caseSensitive()) {
        addPatternToTableWithoutPermutations(index, pattern);
      } else {
        addPatternToTableWithPermutations(index, pattern);
      }

      patterns_.emplace_back(pattern);
    }
  }

 private:
  void addPatternToTableWithoutPermutations(PidIndex pidIndex,
                                            Pattern const& pattern) {
    addPatternToTableForSegment(pidIndex, segmenter_.segment(pattern));
  }

  void addPatternToTableWithPermutations(PidIndex pidIndex,
                                         Pattern const& pattern) noexcept {
    for (auto const segment : segmenter_.permutations(pattern)) {
      addPatternToTableForSegment(pidIndex, segment);
    }
  }

  void addPatternToTableForSegment(PidIndex pidIndex,
                                   SegmentType segment) noexcept {
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
