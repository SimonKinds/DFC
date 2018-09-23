#include "benchmark/benchmark.h"

#include "compact-table.hpp"
#include "util-benchmark.hpp"
#include "util-test.hpp"

using dfc::ImmutablePattern;
using dfc::InputView;
using dfc::Pid;
using dfc::test::createCaseInsensitiveImmutablePattern;
using dfc::test::createImmutablePattern;

namespace {
using CTFourByteIndexer = dfc::CompactTable<uint32_t, 49157, 0x20000>;

dfc::benchmark::CountOnMatcher onMatcher;

void CT_OneByte_FindAllMatches(benchmark::State &state) {
  dfc::CompactTable<uint8_t, 1, 0x100> ct;
  std::string patternValue("x");
  benchmark::DoNotOptimize(&patternValue);

  ct.addPattern(createImmutablePattern(Pid{0}, patternValue.data()));

  InputView input(patternValue.data());
  for (auto _ : state) {
    ct.findAllMatches(input, onMatcher);
  }
}
BENCHMARK(CT_OneByte_FindAllMatches);

void CT_FourByte_FindAllMatches_CaseSensitive(benchmark::State &state) {
  auto patterns = std::make_shared<std::vector<dfc::ImmutablePattern>>();

  CTFourByteIndexer ct;
  std::string patternValue(state.range(0), 'a');
  benchmark::DoNotOptimize(&patternValue);

  ct.addPattern(createImmutablePattern(Pid{0}, patternValue.data()));

  InputView input(patternValue.data());
  for (auto _ : state) {
    ct.findAllMatches(input, onMatcher);
  }
}
BENCHMARK(CT_FourByte_FindAllMatches_CaseSensitive)->Range(4, 1024);

void CT_FourByte_FindAllMatches_CaseInsensitive(benchmark::State &state) {
  auto patterns = std::make_shared<std::vector<dfc::ImmutablePattern>>();

  CTFourByteIndexer ct;
  std::string patternValue(state.range(0), 'a');
  benchmark::DoNotOptimize(&patternValue);

  ct.addPattern(
      createCaseInsensitiveImmutablePattern(Pid{0}, patternValue.data()));

  InputView input(patternValue.data());
  for (auto _ : state) {
    ct.findAllMatches(input, onMatcher);
  }
}
BENCHMARK(CT_FourByte_FindAllMatches_CaseInsensitive)->Range(4, 1024);

}  // namespace
