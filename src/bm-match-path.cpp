#include "benchmark/benchmark.h"

#include "ct.hpp"
#include "df.hpp"
#include "match-path.hpp"
#include "util-benchmark.hpp"
#include "util-test.hpp"

using dfc::CompactTable;
using dfc::DirectFilter;
using dfc::PatternRange;
using dfc::test::createImmutablePattern;

using TwoByteMatchPath =
    dfc::MatchPath<PatternRange<2, 100>, DirectFilter<uint16_t>,
                   CompactTable<uint16_t, 1, 0x100>>;
using TwoByteDfFourByteCtMatchPath =
    dfc::MatchPath<PatternRange<4, 100>, DirectFilter<uint16_t>,
                   CompactTable<uint32_t, 49157, 0x20000>>;

namespace {
void MatchPath_TwoByte_NoHit(benchmark::State& state) {
  TwoByteMatchPath path;

  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    path.match("ab", 2, onMatcher);
    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_TwoByte_NoHit);

void MatchPath_TwoByte_Hit(benchmark::State& state) {
  TwoByteMatchPath path;
  path.addPattern(createImmutablePattern(0, "ab"));

  dfc::benchmark::CountOnMatcher onMatcher;
  for (auto _ : state) {
    path.match("ab", 2, onMatcher);
    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_TwoByte_Hit);

void MatchPath_FourByte_NoHit(benchmark::State& state) {
  TwoByteDfFourByteCtMatchPath path;

  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    path.match("abab", 4, onMatcher);
    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_FourByte_NoHit);

void MatchPath_FourByte_Hit(benchmark::State& state) {
  TwoByteDfFourByteCtMatchPath path;
  path.addPattern(createImmutablePattern(0, "abab"));

  dfc::benchmark::CountOnMatcher onMatcher;
  for (auto _ : state) {
    path.match("abab", 4, onMatcher);
    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_FourByte_Hit);

}  // namespace
