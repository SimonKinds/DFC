#include "catch.hpp"

#include "compact-table.hpp"
#include "flat-direct-filter.hpp"
#include "layered-direct-filter.hpp"
#include "match-path.hpp"
#include "util-test.hpp"

namespace {
using dfc::ImmutablePattern;
using dfc::InputView;
using dfc::Pid;
using dfc::SaveOnMatcher;
using dfc::test::createImmutablePattern;
using dfc::test::createPattern;

using PatternRange = dfc::PatternRange<2, 100>;
using Df = dfc::FlatDirectFilter<uint16_t, 1, uint16_t>;
using LayeredDf =
    dfc::LayeredDirectFilter<dfc::FlatDirectFilter<uint16_t, 1, uint16_t>>;
using Ct = dfc::CompactTable<uint16_t, 1, 100>;

TEST_CASE("No match if no patterns are added") {
  SaveOnMatcher onMatcher;
  dfc::MatchPath<PatternRange, Df, Ct> path;

  path.match(InputView("test"), onMatcher);

  REQUIRE(onMatcher.matchedPids.empty());
}

TEST_CASE("Match if input equals pattern") {
  SaveOnMatcher onMatcher;
  std::string in("test");
  dfc::MatchPath<PatternRange, Df, Ct> path;

  dfc::Pid const pid{123};
  path.addPattern(createImmutablePattern(pid, in.data()));

  path.match(InputView(in.data()), onMatcher);

  REQUIRE(onMatcher.matchedPids.size() == 1);
  REQUIRE(onMatcher.matchedPids[0] == pid);
}

TEST_CASE("Does not add pattern if pattern is outside of range contraints") {
  dfc::MatchPath<PatternRange, Df, Ct> path;

  std::string firstPattern(PatternRange::startInclusive - 1, 'a');
  path.addPattern(createImmutablePattern(dfc::Pid(1), firstPattern.data()));

  std::string secondPattern(PatternRange::endInclusive + 1, 'b');
  path.addPattern(createImmutablePattern(dfc::Pid(2), secondPattern.data()));

  SaveOnMatcher onMatcher;
  path.match(InputView(firstPattern.data()), onMatcher);
  path.match(InputView(secondPattern.data()), onMatcher);

  REQUIRE(onMatcher.matchedPids.empty());
}
TEST_CASE(
    "Extends input by one byte if length is larger than smallest pattern but "
    "shorter than segment type of DF") {
  SaveOnMatcher onMatcher;
  // smallest pattern = 2B
  dfc::MatchPath<dfc::PatternRange<2, 100>, Df, Ct> path;

  path.addPattern(ImmutablePattern(Pid{1}, createPattern("aa")));
  path.addPattern(ImmutablePattern(Pid{2}, createPattern("ab")));
  path.addPattern(ImmutablePattern(Pid{3}, createPattern("ac")));

  path.match(InputView("a"), onMatcher);

  REQUIRE(onMatcher.matchedPids.size() == 3);
  REQUIRE(onMatcher.matchedPids[0] == Pid{1});
  REQUIRE(onMatcher.matchedPids[1] == Pid{2});
  REQUIRE(onMatcher.matchedPids[2] == Pid{3});
}

TEST_CASE("Works with layered direct filter") {
  SaveOnMatcher onMatcher;
  std::string in("test");
  dfc::MatchPath<PatternRange, LayeredDf, Ct> path;

  dfc::Pid const pid{123};
  path.addPattern(createImmutablePattern(pid, in.data()));

  path.match(InputView(in.data()), onMatcher);

  REQUIRE(onMatcher.matchedPids.size() == 1);
  REQUIRE(onMatcher.matchedPids[0] == pid);
}
}  // namespace
