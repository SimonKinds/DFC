#include "benchmark/benchmark.h"

#include "df-initializer.hpp"
#include "indexer.hpp"
#include "segmenter.hpp"

namespace {
dfc::RawPattern twoBytePattern() {
  std::string string_pat("te");
  dfc::RawPattern pat(reinterpret_cast<byte const*>(string_pat.data()),
                      string_pat.size());

  return pat;
}

dfc::RawPattern fiveBytePattern() {
  std::string string_pat("testx");
  dfc::RawPattern pat(reinterpret_cast<byte const*>(string_pat.data()),
                      string_pat.size());

  return pat;
}

static void DF_Initializer_TwoByte_AddPattern(benchmark::State& state) {
  auto const pattern = twoBytePattern();

  dfc::DirectFilterInitializer<uint16_t> init(1, 3);
  for (auto _ : state) {
    init.addPattern(pattern);
    auto const& filter = init.filter();
    benchmark::DoNotOptimize(&filter);
  }
}
BENCHMARK(DF_Initializer_TwoByte_AddPattern);

static void DF_Initializer_FourByteHash_AddPattern(benchmark::State& state) {
  auto const pattern = fiveBytePattern();

  dfc::DirectFilterInitializer<uint32_t, 4909, uint16_t> init(4, 10);
  for (auto _ : state) {
    init.addPattern(pattern);
    auto const& filter = init.filter();
    benchmark::DoNotOptimize(&filter);
  }
}
BENCHMARK(DF_Initializer_FourByteHash_AddPattern);
}  // namespace
