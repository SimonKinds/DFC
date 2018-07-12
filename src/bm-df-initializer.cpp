#include "benchmark/benchmark.h"

#include "df-initializer.hpp"
#include "indexer.hpp"
#include "segmenter.hpp"
#include "util-test.hpp"

using dfc::test::fiveBytePattern;
using dfc::test::twoBytePattern;

namespace {
void DF_Initializer_TwoByte_AddPattern(benchmark::State& state) {
  auto const pattern = twoBytePattern();

  dfc::DirectFilterInitializer<dfc::PatternRange<1, 3>, uint16_t> init;
  for (auto _ : state) {
    init.addPattern(pattern);
    auto const& filter = init.filter();
    benchmark::DoNotOptimize(&filter);
  }
}
BENCHMARK(DF_Initializer_TwoByte_AddPattern);

void DF_Initializer_FourByteHash_AddPattern(benchmark::State& state) {
  auto const pattern = fiveBytePattern();

  dfc::DirectFilterInitializer<dfc::PatternRange<4, 10>, uint32_t, 4909,
                               uint16_t>
      init;
  for (auto _ : state) {
    init.addPattern(pattern);
    auto const& filter = init.filter();
    benchmark::DoNotOptimize(&filter);
  }
}
BENCHMARK(DF_Initializer_FourByteHash_AddPattern);
}  // namespace
