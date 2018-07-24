#include "catch.hpp"

#include "ct.hpp"
#include "util-test.hpp"

using dfc::ImmutablePattern;
using dfc::SaveOnMatcher;
using dfc::test::createCaseInsensitivePattern;
using dfc::test::createPattern;

namespace {

TEST_CASE("CT") {
  SaveOnMatcher onMatcher;

  int const ctSize = 0x100;
  dfc::CompactTable<dfc::PatternRange<1, 10>, uint8_t, 1, ctSize> ct;

  SECTION("Is empty by default") {
    for (int i = 0; i < ctSize; ++i) {
      byte in = i;
      ct.findAllMatches(&in, 1, onMatcher);
    }

    REQUIRE(onMatcher.matchedPids.size() == 0);
  }

  SECTION("Matches if added and equal") {
    auto patternValue = "x";

    dfc::Pid const pid = 1;
    ct.addPattern(ImmutablePattern(pid, createPattern(patternValue)));

    ct.findAllMatches(patternValue, 1, onMatcher);

    REQUIRE(onMatcher.matchedPids.size() == 1);
    REQUIRE(onMatcher.matchedPids[0] == pid);
  }

  SECTION("Does not match if incorrect segment") {
    auto patternValue = "x";

    dfc::Pid const pid = 1;
    ct.addPattern(ImmutablePattern(pid, createPattern(patternValue)));

    ct.findAllMatches("y", 1, onMatcher);

    REQUIRE(onMatcher.matchedPids.size() == 0);
  }

  SECTION("Does not set add pattern if it is outside the range constraint") {
    auto patternValue = "this is a very long pattern";

    dfc::Pid const pid = 1;
    ct.addPattern(ImmutablePattern(pid, createPattern(patternValue)));

    ct.findAllMatches(patternValue, std::strlen(patternValue), onMatcher);

    REQUIRE(onMatcher.matchedPids.size() == 0);
  }

  SECTION("Sets for all variants of input if case insensitive") {
    // ct with two characters pattern
    dfc::CompactTable<dfc::PatternRange<2, 10>, uint16_t, 1, ctSize> ct;
    auto patternValue = "ab";

    dfc::Pid const pid = 1;
    ct.addPattern(
        ImmutablePattern(pid, createCaseInsensitivePattern(patternValue)));

    ct.findAllMatches("ab", 2, onMatcher);
    ct.findAllMatches("aB", 2, onMatcher);
    ct.findAllMatches("Ab", 2, onMatcher);
    ct.findAllMatches("AB", 2, onMatcher);

    REQUIRE(onMatcher.matchedPids.size() == 4);
    REQUIRE(onMatcher.matchedPids[0] == pid);
    REQUIRE(onMatcher.matchedPids[1] == pid);
    REQUIRE(onMatcher.matchedPids[2] == pid);
    REQUIRE(onMatcher.matchedPids[3] == pid);
  }

  SECTION("Multiple patterns") {
    dfc::Pid const firstPatternPid = 1, secondPatternPid = 2;
    SECTION("Can match multiple equal segments") {
      ct.addPattern(ImmutablePattern(firstPatternPid, createPattern("x")));
      ct.addPattern(ImmutablePattern(secondPatternPid, createPattern("x")));

      ct.findAllMatches("x", 1, onMatcher);

      REQUIRE(onMatcher.matchedPids.size() == 2);
      REQUIRE(onMatcher.matchedPids[0] == firstPatternPid);
      REQUIRE(onMatcher.matchedPids[1] == secondPatternPid);
    }

    SECTION("Can match multiple different segments") {
      ct.addPattern(ImmutablePattern(firstPatternPid, createPattern("x")));
      ct.addPattern(ImmutablePattern(secondPatternPid, createPattern("y")));

      ct.findAllMatches("x", 1, onMatcher);
      ct.findAllMatches("y", 1, onMatcher);

      REQUIRE(onMatcher.matchedPids.size() == 2);
      REQUIRE(onMatcher.matchedPids[0] == firstPatternPid);
      REQUIRE(onMatcher.matchedPids[1] == secondPatternPid);
    }
  }
}
}  // namespace
