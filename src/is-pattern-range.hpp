#ifndef DFC_IS_PATTERN_RANGE_HPP
#define DFC_IS_PATTERN_RANGE_HPP

#include <type_traits>

namespace dfc {
template <int, int>
class PatternRange;

template <typename>
struct is_pattern_range : std::false_type {};

template <int a, int b>
struct is_pattern_range<PatternRange<a, b>> : std::true_type {};
}  // namespace dfc

#endif
