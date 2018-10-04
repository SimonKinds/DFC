#ifndef DFC_MATCH_PATH_HPP
#define DFC_MATCH_PATH_HPP

#include <utility>

#include "byte.hpp"
#include "compact-table.hpp"
#include "flat-direct-filter.hpp"
#include "is-compact-table.hpp"
#include "is-direct-filter.hpp"
#include "is-pattern-range.hpp"
#include "on-matcher.hpp"

namespace dfc {

/**
 * A direct filter and compact table covering a certain pattern range
 * First template parameter should be a PatternRange
 * Second template paremeter should be a DirectFilter
 * Third template parameter should be a CompactTable
 */

template <typename PatternRange, typename DF, typename CT>
class MatchPath {
  static_assert(is_pattern_range<PatternRange>::value,
                "First template parameter must be a pattern range");
  static_assert(is_direct_filter<DF>::value,
                "Second template parameter must be a direct filter");
  static_assert(is_compact_table<CT>::value,
                "Last template parameter must be a compact table");
  static_assert(PatternRange::startInclusive ==
                    sizeof(typename CT::SegmentType),
                "The segment type of the CT must be equal in size to the "
                "smallest pattern length");

 private:
  PatternRange const patternRange_{};
  DF df_;
  CT ct_;

 public:
  void addPattern(ImmutablePattern const &pattern) {
    if (patternRange_.includes(pattern)) {
      df_.addPattern(pattern);
      ct_.addPattern(pattern);
    }
  }

  inline void match(InputView const &input, OnMatcher const &onMatcher) const {
    if (doesInputFitInDirectFilter(input) && doesDirectFilterContain(input) &&
        doesInputFitInCompactTable(input)) {
      findAllMatches(input, onMatcher);
    } else if (shouldExtendInput(input)) {
      extendInputAndMatch(input, onMatcher);
    }
  }

 private:
  inline bool doesInputFitInDirectFilter(InputView const &input) const
      noexcept {
    return input.size() >= df_.indexByteCount();
  }

  inline bool doesInputFitInCompactTable(InputView const &input) const
      noexcept {
    return input.size() >= ct_.indexByteCount();
  }

  inline bool doesDirectFilterContain(InputView const &input) const noexcept {
    return df_.contains(input.data());
  }

  /**
   * By marking with __attribute(noinline)__ it can speed up the cases where the
   * compiler decides to inline too much, causing the direct filter hot path to
   * contain too many instructions. Has a great positive impact when single DF,
   * a minor negative impact when 5 DFs.
   *
   * Let the compiler do what it wants for now and check the hot path every now
   * and then.
   *
   * UPDATE: noinline has a 15% speed improvement with three DFs with real data
   */
  __attribute_noinline__ void findAllMatches(InputView const &input,
                                             OnMatcher const &onMatcher) const {
    ct_.findAllMatches(input, onMatcher);
  }

  inline bool shouldExtendInput(InputView const &input) const noexcept {
    const auto size = input.size();
    return size == 1 && PatternRange::startInclusive == 1 &&
           sizeof(typename DF::SegmentType) == 2;
  }

  void extendInputAndMatch(InputView const &inputToExtend,
                           OnMatcher const &onMatcher) const {
    auto const segments = extendInput(inputToExtend);

    for (auto const &segment : segments) {
      InputView const input{segment.data(), static_cast<int>(segment.size())};

      if (doesDirectFilterContain(input)) {
        findAllMatches(input, onMatcher);
      }
    }
  }

  auto extendInput(InputView const &input) const {
    SegmentExtender<typename DF::SegmentType> extender;

    return extender.extend(input.data(), input.size());
  }
};
}  // namespace dfc

#endif
