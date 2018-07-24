#include "benchmark/benchmark.h"

#include "ct.hpp"
#include "util-test.hpp"

using dfc::ImmutablePattern;
using dfc::test::createCaseInsensitivePattern;
using dfc::test::createPattern;

namespace {
using CTFourByteIndexer =
    dfc::CompactTable<dfc::PatternRange<4, 1000000>, uint32_t, 49157, 0x20000>;

struct CountOnMatcher final : public dfc::OnMatcher {
  int mutable matchCount = 0;

  void onMatch(dfc::ImmutablePattern const& pattern) const noexcept final {
    (void)(pattern);
    ++matchCount;
  }
};
CountOnMatcher onMatcher;

void CT_OneByte_FindAllMatches(benchmark::State& state) {
  dfc::CompactTable<dfc::PatternRange<1, 100>, uint8_t, 1, 0x100> ct;
  std::string patternValue("x");
  benchmark::DoNotOptimize(&patternValue);

  ct.addPattern(ImmutablePattern(0, createPattern(patternValue.data())));

  for (auto _ : state) {
    ct.findAllMatches(patternValue.data(), 1, onMatcher);
  }
}
BENCHMARK(CT_OneByte_FindAllMatches);

void CT_FourByte_FindAllMatches_CaseSensitive(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::ImmutablePattern>>();

  CTFourByteIndexer ct;
  std::string patternValue(state.range(0), 'a');
  benchmark::DoNotOptimize(&patternValue);

  ct.addPattern(ImmutablePattern(0, createPattern(patternValue.data())));

  byte const* data = reinterpret_cast<byte const*>(patternValue.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.findAllMatches(data, size, onMatcher);
  }
}
BENCHMARK(CT_FourByte_FindAllMatches_CaseSensitive)->Range(4, 1024);

void CT_FourByte_FindAllMatches_CaseInsensitive(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::ImmutablePattern>>();

  CTFourByteIndexer ct;
  std::string patternValue(state.range(0), 'a');
  benchmark::DoNotOptimize(&patternValue);

  ct.addPattern(
      ImmutablePattern(0, createCaseInsensitivePattern(patternValue.data())));

  byte const* data = reinterpret_cast<byte const*>(patternValue.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.findAllMatches(data, size, onMatcher);
  }
}
BENCHMARK(CT_FourByte_FindAllMatches_CaseInsensitive)->Range(4, 1024);

}  // namespace
