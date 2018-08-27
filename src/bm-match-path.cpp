#include "benchmark/benchmark.h"

#include "ct.hpp"
#include "df.hpp"
#include "match-path.hpp"
#include "util-benchmark.hpp"
#include "util-test.hpp"

using dfc::CompactTable;
using dfc::DirectFilter;
using dfc::InputView;
using dfc::PatternRange;
using dfc::test::createImmutablePattern;

using TwoByteMatchPath =
    dfc::MatchPath<PatternRange<2, 100>, DirectFilter<uint16_t>,
                   CompactTable<uint16_t, 1, 0x100>>;
using TwoByteDfFourByteCtMatchPath =
    dfc::MatchPath<PatternRange<4, 100>, DirectFilter<uint16_t>,
                   CompactTable<uint32_t, 49157, 0x20000>>;

namespace {
void MatchPath_TwoByte_NoHit(benchmark::State &state) {
  TwoByteMatchPath path;

  InputView input("ab");
  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    path.match(input, onMatcher);

    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_TwoByte_NoHit);

void MatchPath_TwoByte_Hit(benchmark::State &state) {
  TwoByteMatchPath path;
  path.addPattern(createImmutablePattern(0, "ab"));

  InputView input("ab");
  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    path.match(input, onMatcher);

    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_TwoByte_Hit);

void MatchPath_FourByte_NoHit(benchmark::State &state) {
  TwoByteDfFourByteCtMatchPath path;

  InputView input("abab");
  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    path.match(input, onMatcher);

    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_FourByte_NoHit);

void MatchPath_FourByte_Hit(benchmark::State &state) {
  TwoByteDfFourByteCtMatchPath path;
  path.addPattern(createImmutablePattern(0, "abab"));

  InputView input("abab");
  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    path.match(input, onMatcher);
    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_FourByte_Hit);

void MatchPath_FourByte_ManyMisses(benchmark::State &state) {
  TwoByteDfFourByteCtMatchPath path;
  path.addPattern(createImmutablePattern(0, "abab"));

  InputView input("abaa");
  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    for (int i = 0; i < 100; ++i) {
      path.match(input, onMatcher);
    }
    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_FourByte_ManyMisses);

void MatchPath_FourByte_ManyHits(benchmark::State &state) {
  TwoByteDfFourByteCtMatchPath path;
  path.addPattern(createImmutablePattern(0, "abab"));

  InputView input("abab");
  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    for (int i = 0; i < 100; ++i) {
      path.match(input, onMatcher);
    }

    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_FourByte_ManyHits);

void MatchPath_FourByte_FiftyPercentHits(benchmark::State &state) {
  TwoByteDfFourByteCtMatchPath path;
  path.addPattern(createImmutablePattern(0, "abab"));

  InputView nonMatchingInput("abaa");
  InputView matchingInput("abab");
  dfc::benchmark::CountOnMatcher onMatcher;

  for (auto _ : state) {
    for (int i = 0; i < 100; ++i) {
      if (i % 2 == 0) {
        path.match(nonMatchingInput, onMatcher);
      } else {
        path.match(matchingInput, onMatcher);
      }
    }

    int count = onMatcher.matchCount;
    benchmark::DoNotOptimize(count);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(MatchPath_FourByte_FiftyPercentHits);

}  // namespace
