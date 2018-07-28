#include "benchmark/benchmark.h"

#include "df.hpp"
#include "util-test.hpp"

using dfc::test::fiveBytePattern;
using dfc::test::twoBytePattern;

namespace {
void DF_TwoByte_AddPattern(benchmark::State& state) {
  auto const pattern = twoBytePattern();

  dfc::DirectFilter<uint16_t> df;
  for (auto _ : state) {
    df.addPattern(pattern);
    auto const& filter = df.filter();
    benchmark::DoNotOptimize(&filter);
  }
}
BENCHMARK(DF_TwoByte_AddPattern);

void DF_FourByteHash_AddPattern(benchmark::State& state) {
  auto const pattern = fiveBytePattern();

  dfc::DirectFilter<uint32_t, 4909, uint16_t> df;
  for (auto _ : state) {
    df.addPattern(pattern);
    auto const& filter = df.filter();
    benchmark::DoNotOptimize(&filter);
  }
}
BENCHMARK(DF_FourByteHash_AddPattern);

void DF_TwoByte_Contains(benchmark::State& state) {
  const auto pattern = twoBytePattern();
  dfc::DirectFilter<uint16_t> df;
  df.addPattern(pattern);

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(DF_TwoByte_Contains);

void DF_FourByteHash_Contains(benchmark::State& state) {
  const auto pattern = fiveBytePattern();

  dfc::DirectFilter<uint32_t, 4909, uint16_t> df;
  df.addPattern(pattern);

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(DF_FourByteHash_Contains);
}  // namespace
