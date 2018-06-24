#ifndef DFC_CT_HPP
#define DFC_CT_HPP

#include <array>
#include <limits>
#include <vector>

#include "byte.hpp"
#include "df-masker.hpp"
#include "indexer.hpp"
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

template <typename SegmentType, SegmentType Hash, int Size, typename OnMatcher>
class CompactTable {
  static_assert(std::is_integral<SegmentType>::value,
                "SegmentType must be integral");

  using Table = std::array<std::vector<CompactTableEntry<SegmentType>>, Size>;
  Table const table_;

  CompactTableIndexer<SegmentType, Hash, Size - 1> const indexer_{};
  Segmenter<SegmentType> const segmenter_{};
  DirectFilterMasker<SegmentType> const masker_{};
  OnMatcher const onMatcher_{};

  std::shared_ptr<std::vector<Pattern> const> patterns_;

 public:
  CompactTable(Table const& table,
               std::shared_ptr<std::vector<Pattern> const> patterns)
      : table_(table), patterns_(std::move(patterns)) {}

  void exactMatching(byte const* const in, int const remaining) const noexcept {
    auto const segment = segmenter_.segment(in);
    auto const index = indexer_.index(segment);

    auto const& bucket = table_[index];

    for (auto const& entry : bucket) {
      if (entry.segment == segment) {
        for (auto const pidIndex : entry.pids) {
          auto const& pattern = (*patterns_)[pidIndex];

          if (matches(in, remaining, pattern)) {
            onMatcher_.onMatch(pattern);
          }
        }

        return;
      }
    }
  }

 private:
  bool matches(byte const* const in, int const remaining,
               Pattern const& pattern) const noexcept {
    // TODO: add case sensitivity
    if (remaining >= pattern.size()) {
      return std::memcmp(in, pattern.data(), pattern.size()) == 0;
    }

    return false;
  }
};
}  // namespace dfc

#endif
