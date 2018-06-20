#include "benchmark/benchmark.h"

#include "indexer.hpp"

using dfc::indexer::Indexer;
using dfc::indexer::df::FourByteHash;
using dfc::indexer::df::TwoByte;

static void BM_TwoByte(benchmark::State& state) {
  uint32_t value = 1337;
  auto ptr = reinterpret_cast<byte*>(&value);
  benchmark::DoNotOptimize(&ptr);
  Indexer<TwoByte> indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(ptr));
  }
}
BENCHMARK(BM_TwoByte);

static void BM_FourByteHash(benchmark::State& state) {
  uint32_t value = 1337;
  auto ptr = reinterpret_cast<byte*>(&value);
  benchmark::DoNotOptimize(&ptr);
  Indexer<FourByteHash> indexer;
  for (auto _ : state) {
    benchmark::DoNotOptimize(indexer.index(ptr));
  }
}
BENCHMARK(BM_FourByteHash);
