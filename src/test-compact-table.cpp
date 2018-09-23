#include "catch.hpp"

#include "compact-table.hpp"
#include "util-test.hpp"

using dfc::ImmutablePattern;
using dfc::InputView;
using dfc::Pid;
using dfc::SaveOnMatcher;
using dfc::test::createCaseInsensitiveImmutablePattern;
using dfc::test::createCaseInsensitivePattern;
using dfc::test::createImmutablePattern;
using dfc::test::createPattern;

namespace {

TEST_CASE("Compact Table") {
  SaveOnMatcher onMatcher;

  int const ctSize = 0x100;
  dfc::CompactTable<uint8_t, 1, ctSize> ct;

  SECTION("Is empty by default") {
    for (int i = 0; i < ctSize; ++i) {
      char in = i;
      ct.findAllMatches(InputView(&in), onMatcher);
    }

    REQUIRE(onMatcher.matchedPids.size() == 0);
  }

  SECTION("Matches if added and equal") {
    auto patternValue = "x";

    Pid const pid{1};
    ct.addPattern(createImmutablePattern(pid, patternValue));

    ct.findAllMatches(InputView(patternValue), onMatcher);

    REQUIRE(onMatcher.matchedPids.size() == 1);
    REQUIRE(onMatcher.matchedPids[0] == pid);
  }

  SECTION("Does not match if incorrect segment") {
    auto patternValue = "x";

    Pid const pid{1};
    ct.addPattern(createImmutablePattern(pid, patternValue));

    ct.findAllMatches(InputView("y"), onMatcher);

    REQUIRE(onMatcher.matchedPids.size() == 0);
  }

  SECTION("Sets for all variants of input if case insensitive") {
    // ct with two characters pattern
    dfc::CompactTable<uint16_t, 1, ctSize> ct;
    auto patternValue = "ab";

    Pid const pid{1};
    ct.addPattern(createCaseInsensitiveImmutablePattern(pid, patternValue));

    ct.findAllMatches(InputView("ab"), onMatcher);
    ct.findAllMatches(InputView("aB"), onMatcher);
    ct.findAllMatches(InputView("Ab"), onMatcher);
    ct.findAllMatches(InputView("AB"), onMatcher);

    REQUIRE(onMatcher.matchedPids.size() == 4);
    REQUIRE(onMatcher.matchedPids[0] == pid);
    REQUIRE(onMatcher.matchedPids[1] == pid);
    REQUIRE(onMatcher.matchedPids[2] == pid);
    REQUIRE(onMatcher.matchedPids[3] == pid);
  }

  SECTION("Multiple patterns") {
    Pid const firstPatternPid{1}, secondPatternPid{2};

    SECTION("Can match multiple equal segments") {
      ct.addPattern(createImmutablePattern(firstPatternPid, "x"));
      ct.addPattern(createImmutablePattern(secondPatternPid, "x"));

      ct.findAllMatches(InputView("x"), onMatcher);

      REQUIRE(onMatcher.matchedPids.size() == 2);
      REQUIRE(onMatcher.matchedPids[0] == firstPatternPid);
      REQUIRE(onMatcher.matchedPids[1] == secondPatternPid);
    }

    SECTION("Can match multiple different segments") {
      ct.addPattern(createImmutablePattern(firstPatternPid, "x"));
      ct.addPattern(createImmutablePattern(secondPatternPid, "y"));

      ct.findAllMatches(InputView("x"), onMatcher);
      ct.findAllMatches(InputView("y"), onMatcher);

      REQUIRE(onMatcher.matchedPids.size() == 2);
      REQUIRE(onMatcher.matchedPids[0] == firstPatternPid);
      REQUIRE(onMatcher.matchedPids[1] == secondPatternPid);
    }
  }
}
}  // namespace
