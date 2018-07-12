#include "benchmark/benchmark.h"

#include "df-initializer.hpp"
#include "util-test.hpp"

using dfc::test::fiveBytePattern;
using dfc::test::twoBytePattern;

namespace {
void DF_TwoByte_Index(benchmark::State& state) {
  const auto pattern = twoBytePattern();
  dfc::DirectFilterInitializer<dfc::PatternRange<1, 3>, uint16_t> init;
  init.addPattern(pattern);
  const auto df = init.df();

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.isSet(data));
  }
}
BENCHMARK(DF_TwoByte_Index);

void DF_FourByteHash_Index(benchmark::State& state) {
  const auto pattern = fiveBytePattern();

  dfc::DirectFilterInitializer<dfc::PatternRange<4, 10>, uint32_t, 4909,
                               uint16_t>
      init;
  init.addPattern(pattern);
  const auto df = init.df();

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.isSet(data));
  }
}
BENCHMARK(DF_FourByteHash_Index);
}  // namespace
