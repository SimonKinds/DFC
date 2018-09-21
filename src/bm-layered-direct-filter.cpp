#include "benchmark/benchmark.h"

#include "flat-direct-filter.hpp"
#include "layered-direct-filter.hpp"
#include "util-test.hpp"

using dfc::FlatDirectFilter;
using dfc::LayeredDirectFilter;
using dfc::test::createPattern;
using dfc::test::fiveBytePattern;
using dfc::test::twoBytePattern;

namespace {
void Layered_DF_Single_TwoByte_Contains(benchmark::State& state) {
  const auto pattern = twoBytePattern();
  LayeredDirectFilter<FlatDirectFilter<uint16_t>> df;
  df.addPattern(pattern);

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(Layered_DF_Single_TwoByte_Contains);

void Layered_DF_TwoByte_FourByteHash_Contains(benchmark::State& state) {
  const auto pattern = fiveBytePattern();

  LayeredDirectFilter<FlatDirectFilter<uint16_t>,
                      FlatDirectFilter<uint32_t, 4909, uint16_t>>
      df;
  df.addPattern(pattern);

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(Layered_DF_TwoByte_FourByteHash_Contains);

void Layered_DF_TwoByte_FourByteHash_Contains_Miss_In_Second(
    benchmark::State& state) {
  LayeredDirectFilter<FlatDirectFilter<uint16_t>,
                      FlatDirectFilter<uint32_t, 4909, uint16_t>>
      df;
  df.addPattern(createPattern("1234"));

  auto data = "1235";

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(Layered_DF_TwoByte_FourByteHash_Contains_Miss_In_Second);
}  // namespace
