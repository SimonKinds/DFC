#include "benchmark/benchmark.h"

#include "execution-loop.hpp"
#include "match-path.hpp"
#include "util-test.hpp"

namespace {
void ExecutionLoop_Single_Character_Hit(benchmark::State& state) {
  dfc::ExecutionLoop<
      dfc::MatchPath<dfc::PatternRange<1, 2>, dfc::FlatDirectFilter<uint8_t>,
                     dfc::CompactTable<uint8_t, 1, 256>>>
      executionLoop;

  executionLoop.addPattern(dfc::test::createImmutablePattern(dfc::Pid{1}, "a"));

  dfc::CountOnMatcher onMatcher;
  for (auto _ : state) {
    executionLoop.match(dfc::InputView("a"), onMatcher);
  }
}
BENCHMARK(ExecutionLoop_Single_Character_Hit);

void ExecutionLoop_Single_Character_NoHit(benchmark::State& state) {
  dfc::ExecutionLoop<
      dfc::MatchPath<dfc::PatternRange<1, 2>, dfc::FlatDirectFilter<uint8_t>,
                     dfc::CompactTable<uint8_t, 1, 256>>>
      executionLoop;

  executionLoop.addPattern(dfc::test::createImmutablePattern(dfc::Pid{1}, "a"));

  dfc::CountOnMatcher onMatcher;
  for (auto _ : state) {
    executionLoop.match(dfc::InputView("b"), onMatcher);
  }
}
BENCHMARK(ExecutionLoop_Single_Character_NoHit);

void ExecutionLoop_Single_Character_Five_MatchPaths_Hit(
    benchmark::State& state) {
  using SingleCharacterMatchPath =
      dfc::MatchPath<dfc::PatternRange<1, 2>, dfc::FlatDirectFilter<uint8_t>,
                     dfc::CompactTable<uint8_t, 1, 256>>;
  dfc::ExecutionLoop<SingleCharacterMatchPath, SingleCharacterMatchPath,
                     SingleCharacterMatchPath, SingleCharacterMatchPath,
                     SingleCharacterMatchPath>
      executionLoop;

  executionLoop.addPattern(dfc::test::createImmutablePattern(dfc::Pid{1}, "a"));

  dfc::CountOnMatcher onMatcher;
  for (auto _ : state) {
    executionLoop.match(dfc::InputView("a"), onMatcher);
  }
}
BENCHMARK(ExecutionLoop_Single_Character_Five_MatchPaths_Hit);

void ExecutionLoop_Single_Character_Five_MatchPaths_NoHit(
    benchmark::State& state) {
  using SingleCharacterMatchPath =
      dfc::MatchPath<dfc::PatternRange<1, 2>, dfc::FlatDirectFilter<uint8_t>,
                     dfc::CompactTable<uint8_t, 1, 256>>;
  dfc::ExecutionLoop<SingleCharacterMatchPath, SingleCharacterMatchPath,
                     SingleCharacterMatchPath, SingleCharacterMatchPath,
                     SingleCharacterMatchPath>
      executionLoop;

  executionLoop.addPattern(dfc::test::createImmutablePattern(dfc::Pid{1}, "a"));

  dfc::CountOnMatcher onMatcher;
  for (auto _ : state) {
    executionLoop.match(dfc::InputView("b"), onMatcher);
  }
}
BENCHMARK(ExecutionLoop_Single_Character_Five_MatchPaths_NoHit);

void ExecutionLoop_Single_Character_LongInput_NoHit(benchmark::State& state) {
  dfc::ExecutionLoop<
      dfc::MatchPath<dfc::PatternRange<1, 2>, dfc::FlatDirectFilter<uint8_t>,
                     dfc::CompactTable<uint8_t, 1, 256>>>
      executionLoop;

  executionLoop.addPattern(dfc::test::createImmutablePattern(dfc::Pid{1}, "a"));
  std::string inputString(1000, 'b');
  dfc::InputView inputView(inputString.data(), inputString.size());

  dfc::CountOnMatcher onMatcher;
  for (auto _ : state) {
    executionLoop.match(inputView, onMatcher);
  }
}
BENCHMARK(ExecutionLoop_Single_Character_LongInput_NoHit);

}  // namespace
