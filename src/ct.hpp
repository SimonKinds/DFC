#ifndef DFC_CT_HPP
#define DFC_CT_HPP

#include <array>
#include <limits>
#include <vector>

#include "byte.hpp"
#include "df-masker.hpp"
#include "indexer.hpp"
#include "matcher.hpp"
#include "on-matcher.hpp"
#include "pattern.hpp"
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

template <typename SegmentType, SegmentType Hash, int Size, typename OnMatcher,
          typename Matcher = MemcmpMatcher>
class CompactTable {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");
  static_assert(std::is_base_of<dfc::OnMatcher, OnMatcher>::value,
                "OnMatcher template parameter is not an OnMatcher");
  static_assert(std::is_base_of<dfc::Matcher, Matcher>::value,
                "Matcher template parameter is not an Matcher");

  using Entry = CompactTableEntry<SegmentType>;
  using Bucket = std::vector<Entry>;
  using Table = std::array<Bucket, Size>;
  Table const table_;

  CompactTableIndexer<SegmentType, Hash, Size - 1> const indexer_{};
  Segmenter<SegmentType> const segmenter_{};
  DirectFilterMasker<SegmentType> const masker_{};

  std::shared_ptr<OnMatcher const> const onMatcher_{};
  Matcher const matcher_{};

  std::shared_ptr<std::vector<Pattern> const> patterns_;

 public:
  CompactTable(Table const& table, std::shared_ptr<OnMatcher const> onMatcher,
               std::shared_ptr<std::vector<Pattern> const> patterns)
      : table_(table),
        onMatcher_(std::move(onMatcher)),
        patterns_(std::move(patterns)) {}

  void exactMatching(char const* const in, int const remaining) const noexcept {
    exactMatching(reinterpret_cast<byte const*>(in), remaining);
  }

  void exactMatching(byte const* const in, int const remaining) const {
    auto const& bucket = getBucket(in);

    findMatchesInBucket(bucket, in, remaining);
  }

 private:
  Bucket const& getBucket(byte const* const in) const {
    auto const segment = segmenter_.segment(in);
    auto const index = indexer_.index(segment);

    return table_[index];
  }

  void findMatchesInBucket(Bucket const& bucket, byte const* const in,
                           int const remaining) const {
    auto const segment = segmenter_.segment(in);

    auto entry = std::cbegin(bucket);
    auto const end = std::cend(bucket);
    bool found = false;
    while (entry != end && !found) {
      if (entry->segment == segment) {
        found = true;

        findMatchesInEntry(*entry, in, remaining);
      }

      ++entry;
    }
  }

  void findMatchesInEntry(Entry const& entry, byte const* const in,
                          int const remaining) const {
    for (auto const pidIndex : entry.pids) {
      auto const& pattern = patterns_->at(pidIndex);

      if (matcher_.matches(in, remaining, pattern)) {
        onMatcher_->onMatch(pattern);
      }
    }
  }
};
}  // namespace dfc

#endif
