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

  using Table = std::array<std::vector<CompactTableEntry<SegmentType>>, Size>;
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
    auto const segment = segmenter_.segment(in);
    auto const index = indexer_.index(segment);

    auto const& bucket = table_[index];

    auto entry = std::cbegin(bucket);
    bool found = false;
    while (entry != std::cend(bucket) && !found) {
      if (entry->segment == segment) {
        found = true;

        for (auto const pidIndex : entry->pids) {
          auto const& pattern = patterns_->at(pidIndex);

          if (matcher_.matches(in, remaining, pattern)) {
            onMatcher_->onMatch(pattern);
          }
        }
      }

      ++entry;
    }
  }
};
}  // namespace dfc

#endif
