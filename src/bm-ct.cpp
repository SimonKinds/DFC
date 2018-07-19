#include "benchmark/benchmark.h"
#include "ct-initializer.hpp"

#include "util-test.hpp"

using dfc::test::createCaseInsensitivePattern;
using dfc::test::createPattern;

namespace {
using CTInitializerFourByteIndexer =
    dfc::CompactTableInitializer<dfc::PatternRange<4, 1000000>, uint32_t, 49157,
                                 0x20000>;

struct CountOnMatcher final : public dfc::OnMatcher {
  int mutable matchCount;
  void onMatch(dfc::ImmutablePattern const& pattern) const noexcept {
    (void)(pattern);
    ++matchCount;
  }
};
CountOnMatcher onMatcher;

void CT_OneByte_ExactMatching(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::ImmutablePattern>>();

  dfc::CompactTableInitializer<dfc::PatternRange<1, 100>, uint8_t, 1, 0x100>
      initializer;
  std::string patternValue("x");
  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct = initializer.ct(patterns);

  for (auto _ : state) {
    ct.exactMatching(patternValue.data(), 1, onMatcher);
  }
}
BENCHMARK(CT_OneByte_ExactMatching);

void CT_FourByte_ExactMatching_CaseSensitive(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::ImmutablePattern>>();

  CTInitializerFourByteIndexer initializer;
  std::string patternValue(state.range(0), 'a');
  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createPattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct = initializer.ct(patterns);

  byte const* data = reinterpret_cast<byte const*>(patternValue.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.exactMatching(data, size, onMatcher);
  }
}
BENCHMARK(CT_FourByte_ExactMatching_CaseSensitive)->Range(4, 1024);

void CT_FourByte_ExactMatching_CaseInsensitive(benchmark::State& state) {
  auto patterns = std::make_shared<std::vector<dfc::ImmutablePattern>>();

  CTInitializerFourByteIndexer initializer;
  std::string patternValue(state.range(0), 'a');
  benchmark::DoNotOptimize(&patternValue);

  patterns->emplace_back(0, createCaseInsensitivePattern(patternValue.data()));
  int const patternIndex = 0;
  initializer.addPattern(patternIndex, patterns->at(patternIndex));

  auto const ct = initializer.ct(patterns);

  byte const* data = reinterpret_cast<byte const*>(patternValue.data());
  int size = patternValue.size();

  benchmark::DoNotOptimize(&data);
  benchmark::DoNotOptimize(&size);
  for (auto _ : state) {
    ct.exactMatching(data, size, onMatcher);
  }
}
BENCHMARK(CT_FourByte_ExactMatching_CaseInsensitive)->Range(4, 1024);

}  // namespace
