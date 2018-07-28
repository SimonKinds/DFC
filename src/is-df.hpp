#ifndef DFC_IS_DIRECT_FILTER_HPP
#define DFC_IS_DIRECT_FILTER_HPP

#include <type_traits>

namespace dfc {
template <typename PatternRange, typename SegmentType, SegmentType Hash,
          typename IndexType>
class DirectFilter;

template <typename>
struct is_direct_filter : std::false_type {};

template <typename PatternRange, typename SegmentType, SegmentType Hash,
          typename IndexType>
struct is_direct_filter<
    DirectFilter<PatternRange, SegmentType, Hash, IndexType>> : std::true_type {
};
}  // namespace dfc

#endif
