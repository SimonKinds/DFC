#include "benchmark/benchmark.h"
#include "ct-initializer.hpp"

#include "util-test.hpp"

using dfc::test::createPattern;

namespace {
using CTInitializerFourByteIndexer =
    dfc::CompactTableInitializer<uint32_t, 49157, 0x20000>;

struct CountOnMatcher : public dfc::OnMatcher {
  int mutable matchCount;
  void onMatch(dfc::Pattern const& pattern) const noexcept final {
    (void)(pattern);
    ++matchCount;
  }
};
auto onMatcher = std::make_shared<CountOnMatcher>();

void CT_OneByte_ExactMatching_Match_Memcmp(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

  dfc::CompactTableInitializer<uint8_t, 1, 0x100> initializer;
  std::string patternValue("x");
  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct =
      initializer.ct<CountOnMatcher, dfc::MemcmpMatcher>(onMatcher, patterns);

  for (auto _ : state) {
    ct.exactMatching(patternValue.data(), 1);
  }
}
BENCHMARK(CT_OneByte_ExactMatching_Match_Memcmp);

void CT_OneByte_ExactMatching_Match_Loop(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

  dfc::CompactTableInitializer<uint8_t, 1, 0x100> initializer;
  std::string patternValue("x");
  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct =
      initializer.ct<CountOnMatcher, dfc::LoopMatcher>(onMatcher, patterns);

  for (auto _ : state) {
    ct.exactMatching(patternValue.data(), 1);
  }
}
BENCHMARK(CT_OneByte_ExactMatching_Match_Loop);

void CT_FourByte_ExactMatching_Match_Memcmp(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

  CTInitializerFourByteIndexer initializer;
  std::string patternValue(state.range(0), 'a');
  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct =
      initializer.ct<CountOnMatcher, dfc::MemcmpMatcher>(onMatcher, patterns);

  byte const* data = reinterpret_cast<byte const*>(patternValue.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.exactMatching(data, size);
  }
}
BENCHMARK(CT_FourByte_ExactMatching_Match_Memcmp)->Range(4, 1024);

void CT_FourByte_ExactMatching_Match_Loop(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

  CTInitializerFourByteIndexer initializer;
  std::string patternValue(state.range(0), 'a');
  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct =
      initializer.ct<CountOnMatcher, dfc::LoopMatcher>(onMatcher, patterns);

  byte const* data = reinterpret_cast<byte const*>(patternValue.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.exactMatching(data, size);
  }
}

BENCHMARK(CT_FourByte_ExactMatching_Match_Loop)->Range(4, 1024);

void CT_FourByte_ExactMatching_NoMatch_Start_Memcmp(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

  CTInitializerFourByteIndexer initializer;
  int range = state.range(0);
  std::string patternValue(4, 'a');
  patternValue += 'b';
  if (range > 4) {
    patternValue += std::string(range - 5, 'a');
  }
  std::string input(range, 'a');

  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct =
      initializer.ct<CountOnMatcher, dfc::MemcmpMatcher>(onMatcher, patterns);

  byte const* data = reinterpret_cast<byte const*>(input.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.exactMatching(data, size);
  }
}
BENCHMARK(CT_FourByte_ExactMatching_NoMatch_Start_Memcmp)->Range(4, 1024);

void CT_FourByte_ExactMatching_NoMatch_Start_Loop(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

  CTInitializerFourByteIndexer initializer;
  int range = state.range(0);
  std::string patternValue(4, 'a');
  patternValue += 'b';
  if (range > 4) {
    patternValue += std::string(range - 5, 'a');
  }
  std::string input(range, 'a');

  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct =
      initializer.ct<CountOnMatcher, dfc::LoopMatcher>(onMatcher, patterns);

  byte const* data = reinterpret_cast<byte const*>(input.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.exactMatching(data, size);
  }
}
BENCHMARK(CT_FourByte_ExactMatching_NoMatch_Start_Loop)->Range(4, 1024);

void CT_FourByte_ExactMatching_NoMatch_Half_Memcmp(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

  CTInitializerFourByteIndexer initializer;
  int range = state.range(0);
  std::string patternValue(range / 2, 'a');
  patternValue += 'b';
  patternValue += std::string(range / 2 - 1, 'a');
  std::string input(range, 'a');

  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct =
      initializer.ct<CountOnMatcher, dfc::MemcmpMatcher>(onMatcher, patterns);

  byte const* data = reinterpret_cast<byte const*>(input.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.exactMatching(data, size);
  }
}
BENCHMARK(CT_FourByte_ExactMatching_NoMatch_Half_Memcmp)->Range(4, 1024);

void CT_FourByte_ExactMatching_NoMatch_Half_Loop(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

  CTInitializerFourByteIndexer initializer;
  int range = state.range(0);
  std::string patternValue(range / 2, 'a');
  patternValue += 'b';
  patternValue += std::string(range / 2 - 1, 'a');
  std::string input(range, 'a');

  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct =
      initializer.ct<CountOnMatcher, dfc::LoopMatcher>(onMatcher, patterns);

  byte const* data = reinterpret_cast<byte const*>(input.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.exactMatching(data, size);
  }
}
BENCHMARK(CT_FourByte_ExactMatching_NoMatch_Half_Loop)->Range(4, 1024);

}  // namespace
