#include "benchmark/benchmark.h"

#include "df-collection.hpp"
#include "util-test.hpp"

using dfc::DirectFilter;
using dfc::DirectFilterCollection;
using dfc::test::createPattern;
using dfc::test::fiveBytePattern;
using dfc::test::twoBytePattern;

namespace {
void DF_Collection_Single_TwoByte_Contains(benchmark::State& state) {
  const auto pattern = twoBytePattern();
  DirectFilterCollection<DirectFilter<uint16_t>> df;
  df.addPattern(pattern);

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(DF_Collection_Single_TwoByte_Contains);

void DF_Collection_TwoByte_FourByteHash_Contains(benchmark::State& state) {
  const auto pattern = fiveBytePattern();

  DirectFilterCollection<DirectFilter<uint16_t>,
                         DirectFilter<uint32_t, 4909, uint16_t>>
      df;
  df.addPattern(pattern);

  const auto data = pattern.data();

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(DF_Collection_TwoByte_FourByteHash_Contains);

void DF_Collection_TwoByte_FourByteHash_Contains_Miss_In_Second(
    benchmark::State& state) {
  DirectFilterCollection<DirectFilter<uint16_t>,
                         DirectFilter<uint32_t, 4909, uint16_t>>
      df;
  df.addPattern(createPattern("1234"));

  auto data = "1235";

  benchmark::DoNotOptimize(&df);
  benchmark::DoNotOptimize(&data);

  for (auto _ : state) {
    benchmark::DoNotOptimize(df.contains(data));
  }
}
BENCHMARK(DF_Collection_TwoByte_FourByteHash_Contains_Miss_In_Second);
}  // namespace
