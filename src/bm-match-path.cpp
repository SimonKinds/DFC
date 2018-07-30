#include "benchmark/benchmark.h"

#include "ct.hpp"
#include "df.hpp"
#include "match-path.hpp"
#include "util-benchmark.hpp"

using dfc::CompactTable;
using dfc::DirectFilter;
using dfc::PatternRange;

namespace {
void MatchPath_TwoByte_Empty(benchmark::State& state) {
  dfc::MatchPath<PatternRange<2, 100>, DirectFilter<uint16_t>,
                 CompactTable<PatternRange<2, 100>, uint16_t, 1, 0x100>>
      path;

  auto input = "ab";
  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    path.match(input, 2, onMatcher);
  }
}
BENCHMARK(MatchPath_TwoByte_Empty);
}  // namespace
