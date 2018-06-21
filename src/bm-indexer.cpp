#include <limits>

#include "benchmark/benchmark.h"

#include "indexer.hpp"

namespace {
static void BM_DF_TwoByte(benchmark::State& state) {
  uint16_t segment = std::numeric_limits<uint16_t>::max();
  benchmark::DoNotOptimize(&segment);
  dfc::TwoByteDfIndexer indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(segment));
  }
}
BENCHMARK(BM_DF_TwoByte);

static void BM_DF_FourByteHash(benchmark::State& state) {
  uint32_t segment = std::numeric_limits<uint32_t>::max();
  benchmark::DoNotOptimize(&segment);
  dfc::FourByteHashDfIndexer indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(segment));
  }
}
BENCHMARK(BM_DF_FourByteHash);

static void BM_CT_IndexerSmall(benchmark::State& state) {
  auto segment = std::numeric_limits<uint8_t>::max();
  benchmark::DoNotOptimize(&segment);
  dfc::CtIndexer<uint8_t, 1, 0xff> indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(segment));
  }
}
BENCHMARK(BM_CT_IndexerSmall);

static void BM_CT_IndexerLarge(benchmark::State& state) {
  auto segment = std::numeric_limits<uint32_t>::max();
  benchmark::DoNotOptimize(&segment);
  dfc::CtIndexer<uint16_t, 44257, 0x1fff> indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(segment));
  }
}
BENCHMARK(BM_CT_IndexerLarge);
}  // namespace
