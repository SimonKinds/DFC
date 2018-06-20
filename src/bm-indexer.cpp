#include "benchmark/benchmark.h"

#include "indexer.hpp"

static void BM_DF_TwoByte(benchmark::State& state) {
  uint32_t value = 1337;
  auto ptr = reinterpret_cast<byte*>(&value);
  benchmark::DoNotOptimize(&ptr);
  dfc::TwoByteDfIndexer indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(ptr));
  }
}
BENCHMARK(BM_DF_TwoByte);

static void BM_DF_FourByteHash(benchmark::State& state) {
  uint32_t value = 1337;
  auto ptr = reinterpret_cast<byte*>(&value);
  benchmark::DoNotOptimize(&ptr);
  dfc::FourByteHashDfIndexer indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(ptr));
  }
}
BENCHMARK(BM_DF_FourByteHash);

static void BM_CT_IndexerSmall(benchmark::State& state) {
  uint32_t value = 1337;
  auto ptr = reinterpret_cast<byte*>(&value);
  benchmark::DoNotOptimize(&ptr);
  dfc::CtIndexer<uint8_t, 1, 0xff> indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(ptr));
  }
}
BENCHMARK(BM_CT_IndexerSmall);

static void BM_CT_IndexerLarge(benchmark::State& state) {
  uint32_t value = 1337;
  auto ptr = reinterpret_cast<byte*>(&value);
  benchmark::DoNotOptimize(&ptr);
  dfc::CtIndexer<uint16_t, 44257, 0x1fff> indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(ptr));
  }
}
BENCHMARK(BM_CT_IndexerLarge);
