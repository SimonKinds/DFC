#ifndef DFC_IS_COMPACT_TABLE_HPP
#define DFC_IS_COMPACT_TABLE_HPP

#include <type_traits>

namespace dfc {

template <typename PatternRange, typename SegmentType, SegmentType Hash,
          int Size>
class CompactTable;

template <typename>
struct is_compact_table : std::false_type {};

template <typename PatternRange, typename SegmentType, SegmentType Hash,
          int Size>
struct is_compact_table<CompactTable<PatternRange, SegmentType, Hash, Size>>
    : std::true_type {};

}  // namespace dfc

#endif
