#include "catch.hpp"

#include "ct-initializer.hpp"
#include "util-test.hpp"

using dfc::SaveOnMatcher;
using dfc::test::createPattern;

namespace {

TEST_CASE("CT") {
  auto onMatcher = std::make_shared<SaveOnMatcher>();
  auto patterns = std::make_shared<std::vector<dfc::ImmutablePattern>>();

  int const ctSize = 0x100;
  dfc::CompactTableInitializer<dfc::PatternRange<1, 10>, uint8_t, 1, ctSize>
      initializer;

  SECTION("Is empty by default") {
    auto const ct = initializer.ct(onMatcher, patterns);

    for (int i = 0; i < ctSize; ++i) {
      byte in = i;
      ct.exactMatching(&in, 1);
    }

    REQUIRE(onMatcher->matchedPids.size() == 0);
  }

  SECTION("Matches if added and equal") {
    auto patternValue = "x";

    dfc::Pid const pid = 1;
    patterns->emplace_back(pid, createPattern(patternValue));
    int const patternIndex = 0;
    initializer.addPattern(patternIndex, patterns->at(patternIndex));

    auto const ct = initializer.ct(onMatcher, patterns);
    ct.exactMatching(patternValue, 1);

    REQUIRE(onMatcher->matchedPids.size() == 1);
    REQUIRE(onMatcher->matchedPids[0] == pid);
  }

  SECTION("Does not match if incorrect segment") {
    auto patternValue = "x";

    dfc::Pid const pid = 1;
    patterns->emplace_back(pid, createPattern(patternValue));
    int const patternIndex = 0;
    initializer.addPattern(patternIndex, patterns->at(patternIndex));

    auto const ct = initializer.ct(onMatcher, patterns);
    ct.exactMatching("y", 1);

    REQUIRE(onMatcher->matchedPids.size() == 0);
  }
  SECTION("Does not set add pattern if it is outside the range constraint") {
    auto patternValue = "this is a very long pattern";

    dfc::Pid const pid = 1;
    patterns->emplace_back(pid, createPattern(patternValue));
    int const patternIndex = 0;
    initializer.addPattern(patternIndex, patterns->at(patternIndex));

    auto const ct = initializer.ct(onMatcher, patterns);
    ct.exactMatching(patternValue, std::strlen(patternValue));

    REQUIRE(onMatcher->matchedPids.size() == 0);
  }

  SECTION("Multiple patterns") {
    dfc::Pid const firstPatternPid = 1, secondPatternPid = 2;
    SECTION("Can match multiple equal segments") {
      patterns->emplace_back(firstPatternPid, createPattern("x"));
      patterns->emplace_back(secondPatternPid, createPattern("x"));
      initializer.addPattern(0, patterns->at(0));
      initializer.addPattern(1, patterns->at(1));

      auto const ct = initializer.ct(onMatcher, patterns);
      ct.exactMatching("x", 1);

      REQUIRE(onMatcher->matchedPids.size() == 2);
      REQUIRE(onMatcher->matchedPids[0] == firstPatternPid);
      REQUIRE(onMatcher->matchedPids[1] == secondPatternPid);
    }

    SECTION("Can match multiple different segments") {
      patterns->emplace_back(firstPatternPid, createPattern("x"));
      patterns->emplace_back(secondPatternPid, createPattern("y"));
      initializer.addPattern(0, patterns->at(0));
      initializer.addPattern(1, patterns->at(1));

      auto const ct = initializer.ct(onMatcher, patterns);
      ct.exactMatching("x", 1);
      ct.exactMatching("y", 1);

      REQUIRE(onMatcher->matchedPids.size() == 2);
      REQUIRE(onMatcher->matchedPids[0] == firstPatternPid);
      REQUIRE(onMatcher->matchedPids[1] == secondPatternPid);
    }
  }
}
}  // namespace
