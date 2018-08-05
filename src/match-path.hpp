#ifndef DFC_MATCH_PATH_HPP
#define DFC_MATCH_PATH_HPP

#include <utility>

#include "byte.hpp"
#include "ct.hpp"
#include "df.hpp"
#include "is-ct.hpp"
#include "is-df.hpp"
#include "is-pattern-range.hpp"
#include "on-matcher.hpp"

namespace dfc {

template <typename PatternRange, typename DF, typename CT> class MatchPath {
  static_assert(is_pattern_range<PatternRange>::value,
                "First template parameter must be a pattern range");
  static_assert(is_direct_filter<DF>::value,
                "Second template parameter must be a direct filter");
  static_assert(is_compact_table<CT>::value,
                "Last template parameter must be a compact table");
  static_assert(PatternRange::startInclusive ==
                    sizeof(typename CT::segment_type),
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
    if (df_.contains(input.data())) {
      ct_.findAllMatches(input, onMatcher);
    }
  }
};
} // namespace dfc

#endif
