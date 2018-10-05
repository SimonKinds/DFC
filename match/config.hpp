#ifndef DFC_MATCH_CONFIG_HPP
#define DFC_MATCH_CONFIG_HPP

#include "compact-table.hpp"
#include "execution-loop.hpp"
#include "flat-direct-filter.hpp"
#include "layered-direct-filter.hpp"
#include "match-path.hpp"

/**
 * Use this file to setup easy aliases to construct a CustomExecutionLoop.
 * The custom execution loop is defined at the end of this file, and should be
 * of type dfc::ExecutionLoop.
 */

namespace dfc {
namespace match {

/**
 * The first direct filter is indexed by 2 bytes (16 bits)
 *
 * The second direct filter uses 4 bytes to index, multiplies it
 * with 8387 as a simple hash, and then casts the result to 2 bytes
 */
using TwoByteDirectFilter = FlatDirectFilter<uint16_t>;
using FourByteDirectFilterWithMask = FlatDirectFilter<uint32_t, 8387, uint16_t>;

/**
 * The first compact table is indexed by one byte, has no hash (1), and the
 * table is of size 256 (0x100)
 *
 *
 * The second compact table that is indexed by four byte, has a hash of 8389,
 * and the table is of size 0x20000. The index will be masked to the maximum
 * index of the table (0x20000 - 1)
 */
using OneByteCompactTable = CompactTable<uint8_t, 1, 0x100>;
using FourByteCompactTable = CompactTable<uint32_t, 8389, 0x20000>;

/**
 * A "match path" is a path from the input, through one or more direct filters
 * into the compact table.
 *
 * The first match path will contain patterns of size 1, 2 and 3 characters,
 * and use a direct filter indexed by two bytes, and a compact tabled indexed by
 * one byte (the size of the smallest pattern in this path)
 *
 * The second match path will contain patterns of size 4 through 64 (inclusive),
 * but will be filtered by two direct filters layered on each other. If the
 * first direct filter matches, the second filter will be attempted.
 */
using FirstMatchPath =
    MatchPath<PatternRange<1, 3>, TwoByteDirectFilter, OneByteCompactTable>;
using SecondMatchPath = MatchPath<
    PatternRange<4, 64>,
    LayeredDirectFilter<TwoByteDirectFilter, FourByteDirectFilterWithMask>,
    FourByteCompactTable>;

/**
 * It's required to alias a type called CustomExecutionLoop, and it should be
 * of type ExecutionLoop. The template parameters to the ExecutionLoop has to
 * of type MatchPath
 */
using CustomExecutionLoop = ExecutionLoop<FirstMatchPath, SecondMatchPath>;

}  // namespace match
}  // namespace dfc

#endif
