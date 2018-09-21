#include "benchmark/benchmark.h"

#include "flat-direct-filter.hpp"
#include "util-test.hpp"

using dfc::test::fiveBytePattern;
using dfc::test::twoBytePattern;

namespace {
void Flat_DF_TwoByte_AddPattern(benchmark::State& state) {
  auto const pattern = twoBytePattern();

  dfc::FlatDirectFilter<uint16_t> df;
  for (auto _ : state) {
    df.addPattern(pattern);
    auto const& filter = df.filter();
    benchmark::DoNotOptimize(&filter);
  }
}
BENCHMARK(Flat_DF_TwoByte_AddPattern);

void Flat_DF_FourByteHash_AddPattern(benchmark::State& state) {
  auto const pattern = fiveBytePattern();

  dfc::FlatDirectFilter<uint32_t, 4909, uint16_t> df;
  for (auto _ : state) {
    df.addPattern(pattern);
    auto const& filter = df.filter();
    benchmark::DoNotOptimize(&filter);
  }
}
BENCHMARK(Flat_DF_FourByteHash_AddPattern);

void Flat_DF_TwoByte_Contains(benchmark::State& state) {
  const auto pattern = twoBytePattern();
  dfc::FlatDirectFilter<uint16_t> df;
  df.addPattern(pattern);

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(Flat_DF_TwoByte_Contains);

void Flat_DF_FourByteHash_Contains(benchmark::State& state) {
  const auto pattern = fiveBytePattern();

  dfc::FlatDirectFilter<uint32_t, 4909, uint16_t> df;
  df.addPattern(pattern);

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(Flat_DF_FourByteHash_Contains);
}  // namespace
