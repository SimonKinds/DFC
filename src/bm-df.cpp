#include "benchmark/benchmark.h"

#include "df-initializer.hpp"

namespace {
dfc::RawPattern twoBytePattern() {
  std::string string_pat("te");
  dfc::RawPattern pat(reinterpret_cast<const byte*>(string_pat.data()),
                      string_pat.size());

  return pat;
}

dfc::RawPattern fiveBytePattern() {
  std::string string_pat("testx");
  dfc::RawPattern pat(reinterpret_cast<const byte*>(string_pat.data()),
                      string_pat.size());

  return pat;
}

static void DF_TwoByte_Index(benchmark::State& state) {
  const auto pattern = twoBytePattern();
  dfc::DirectFilterInitializer<uint16_t> init(1, 3);
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

static void DF_FourByteHash_Index(benchmark::State& state) {
  const auto pattern = fiveBytePattern();

  dfc::DirectFilterInitializer<uint32_t, 4909, uint16_t> init(4, 10);
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
