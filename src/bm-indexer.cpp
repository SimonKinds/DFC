#include <limits>

#include "benchmark/benchmark.h"

#include "indexer.hpp"

namespace {
using TwoByteDirectFilterIndexer =
    dfc::DirectFilterIndexer<uint16_t, 1, uint16_t>;
using FourByteHashDirectFilterIndexer =
    dfc::DirectFilterIndexer<uint32_t, 4909, uint16_t>;

static void DF_TwoByte(benchmark::State& state) {
  uint16_t segment = std::numeric_limits<uint16_t>::max();
  benchmark::DoNotOptimize(&segment);
  TwoByteDirectFilterIndexer indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(segment));
  }
}
BENCHMARK(DF_TwoByte);

static void DF_FourByteHash(benchmark::State& state) {
  uint32_t segment = std::numeric_limits<uint32_t>::max();
  benchmark::DoNotOptimize(&segment);
  FourByteHashDirectFilterIndexer indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(segment));
  }
}
BENCHMARK(DF_FourByteHash);

static void CT_IndexerSmall(benchmark::State& state) {
  auto segment = std::numeric_limits<uint8_t>::max();
  benchmark::DoNotOptimize(&segment);
  dfc::CompactTableIndexer<uint8_t, 1, 0xff> indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(segment));
  }
}
BENCHMARK(CT_IndexerSmall);

static void CT_IndexerLarge(benchmark::State& state) {
  auto segment = std::numeric_limits<uint32_t>::max();
  benchmark::DoNotOptimize(&segment);
  dfc::CompactTableIndexer<uint16_t, 44257, 0x1fff> indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(segment));
  }
}
BENCHMARK(CT_IndexerLarge);
}  // namespace
