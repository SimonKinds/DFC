#include "benchmark/benchmark.h"

#include "df-initializer.hpp"
#include "indexer.hpp"
#include "segmenter.hpp"

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

static void DF_Initializer_TwoByte_AddPattern(benchmark::State& state) {
  const auto pattern = twoBytePattern();

  dfc::DfInitializer<dfc::TwoByteDfIndexer, dfc::Segmenter<2>> init(1, 3);
  for (auto _ : state) {
    init.addPattern(pattern);
    auto df = init.df();
    benchmark::DoNotOptimize(df.get());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(DF_Initializer_TwoByte_AddPattern);

static void DF_Initializer_FourByteHash_AddPattern(benchmark::State& state) {
  const auto pattern = fiveBytePattern();

  dfc::DfInitializer<dfc::FourByteHashDfIndexer, dfc::Segmenter<4>> init(4, 10);
  for (auto _ : state) {
    init.addPattern(pattern);
    auto df = init.df();
    benchmark::DoNotOptimize(df.get());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(DF_Initializer_FourByteHash_AddPattern);
