#include "catch.hpp"

#include "execution-loop.hpp"
#include "match-path.hpp"
#include "util-test.hpp"

TEST_CASE("Can match single character") {
  dfc::ExecutionLoop<
      dfc::MatchPath<dfc::PatternRange<1, 2>, dfc::FlatDirectFilter<uint8_t>,
                     dfc::CompactTable<uint8_t, 1, 256>>>
      executionLoop;

  executionLoop.addPattern(dfc::test::createImmutablePattern(dfc::Pid{1}, "a"));

  dfc::SaveOnMatcher onMatcher;
  executionLoop.match(dfc::InputView("a"), onMatcher);

  REQUIRE(onMatcher.matchedPids.size() == 1);
  REQUIRE(onMatcher.matchedPids[0] == dfc::Pid{1});
}

TEST_CASE("Will match all occurences in input") {
  dfc::ExecutionLoop<
      dfc::MatchPath<dfc::PatternRange<1, 2>, dfc::FlatDirectFilter<uint8_t>,
                     dfc::CompactTable<uint8_t, 1, 256>>>
      executionLoop;

  executionLoop.addPattern(dfc::test::createImmutablePattern(dfc::Pid{1}, "a"));
  executionLoop.addPattern(dfc::test::createImmutablePattern(dfc::Pid{2}, "b"));

  dfc::SaveOnMatcher onMatcher;
  executionLoop.match(dfc::InputView("ab"), onMatcher);

  REQUIRE(onMatcher.matchedPids.size() == 2);
  REQUIRE(onMatcher.matchedPids[0] == dfc::Pid{1});
  REQUIRE(onMatcher.matchedPids[1] == dfc::Pid{2});
}

TEST_CASE("Uses all given match paths") {
  dfc::ExecutionLoop<
      dfc::MatchPath<dfc::PatternRange<1, 3>, dfc::FlatDirectFilter<uint8_t>,
                     dfc::CompactTable<uint8_t, 1, 256>>,
      dfc::MatchPath<dfc::PatternRange<4, 10>, dfc::FlatDirectFilter<uint16_t>,
                     dfc::CompactTable<uint32_t, 1, 256>>>
      executionLoop;

  dfc::Pid singleCharacterPid{1};
  executionLoop.addPattern(
      dfc::test::createImmutablePattern(singleCharacterPid, "a"));

  dfc::Pid multipleCharacterPid{2};
  executionLoop.addPattern(
      dfc::test::createImmutablePattern(multipleCharacterPid, "abcd"));

  dfc::SaveOnMatcher onMatcher;
  executionLoop.match(dfc::InputView("abcd abcd"), onMatcher);

  REQUIRE(onMatcher.matchedPids.size() == 4);
  REQUIRE(onMatcher.matchedPids[0] == singleCharacterPid);
  REQUIRE(onMatcher.matchedPids[1] == multipleCharacterPid);
  REQUIRE(onMatcher.matchedPids[2] == singleCharacterPid);
  REQUIRE(onMatcher.matchedPids[3] == multipleCharacterPid);
}
