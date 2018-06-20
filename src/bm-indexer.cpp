#include "benchmark/benchmark.h"

#include "indexer.hpp"

static void BM_TwoByte(benchmark::State& state) {
  uint32_t value = 1337;
  auto ptr = reinterpret_cast<byte*>(&value);
  benchmark::DoNotOptimize(&ptr);
  dfc::TwoByteDfIndexer indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(ptr));
  }
}
BENCHMARK(BM_TwoByte);

static void BM_FourByteHash(benchmark::State& state) {
  uint32_t value = 1337;
  auto ptr = reinterpret_cast<byte*>(&value);
  benchmark::DoNotOptimize(&ptr);
  dfc::FourByteHashDfIndexer indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(ptr));
  }
}
BENCHMARK(BM_FourByteHash);
