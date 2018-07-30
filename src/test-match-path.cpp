#include "catch.hpp"

#include "ct.hpp"
#include "df.hpp"
#include "match-path.hpp"
#include "util-test.hpp"

namespace {
using dfc::ImmutablePattern;
using dfc::SaveOnMatcher;
using dfc::test::createImmutablePattern;

using PatternRange = dfc::PatternRange<2, 100>;
using Df = dfc::DirectFilter<uint16_t, 1, uint16_t>;
using Ct = dfc::CompactTable<uint16_t, 1, 100>;

TEST_CASE("No match if no patterns are added") {
  SaveOnMatcher onMatcher;
  dfc::MatchPath<PatternRange, Df, Ct> path;

  std::string in("test");
  path.match(in.data(), in.size(), onMatcher);

  REQUIRE(onMatcher.matchedPids.empty());
}

TEST_CASE("Match if input equals pattern") {
  SaveOnMatcher onMatcher;
  std::string in("test");
  dfc::MatchPath<PatternRange, Df, Ct> path;

  dfc::Pid const pid = 123;
  path.addPattern(createImmutablePattern(pid, in.data()));

  path.match(in.data(), in.size(), onMatcher);

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
  path.match(firstPattern.data(), firstPattern.size(), onMatcher);
  path.match(secondPattern.data(), secondPattern.size(), onMatcher);

  REQUIRE(onMatcher.matchedPids.empty());
}
}  // namespace
