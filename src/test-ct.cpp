#include "catch.hpp"

#include "ct-initializer.hpp"
#include "util-test.hpp"

using dfc::test::createPattern;

namespace {
struct TestOnMatcher : public dfc::OnMatcher<TestOnMatcher> {
  std::vector<dfc::Pid> mutable matchedPids;

  void onMatch(dfc::Pattern const& pattern) const {
    matchedPids.emplace_back(pattern.pid());
  }
};

TEST_CASE("CT") {
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

  int const ctSize = 0x100;
  dfc::CompactTableInitializer<uint8_t, 1, ctSize> initializer;

  SECTION("Is empty by default") {
    auto const ct = initializer.ct<TestOnMatcher>(patterns);

    for (int i = 0; i < ctSize; ++i) {
      byte in = i;
      ct.exactMatching(&in, 1);
    }

    REQUIRE(ct.onMatcher().matchedPids.size() == 0);
  }

  SECTION("Matches if added and equal") {
    auto patternValue = "x";

    dfc::Pid const pid = 1;
    patterns->emplace_back(pid, createPattern(patternValue));
    int const patternIndex = 0;
    initializer.addPattern(patternIndex, patterns->at(patternIndex));

    auto const ct = initializer.ct<TestOnMatcher>(patterns);
    ct.exactMatching(patternValue, 1);

    REQUIRE(ct.onMatcher().matchedPids.size() == 1);
    REQUIRE(ct.onMatcher().matchedPids[0] == pid);
  }

  SECTION("Does not match if incorrect segment") {
    auto patternValue = "x";

    dfc::Pid const pid = 1;
    patterns->emplace_back(pid, createPattern(patternValue));
    int const patternIndex = 0;
    initializer.addPattern(patternIndex, patterns->at(patternIndex));

    auto const ct = initializer.ct<TestOnMatcher>(patterns);
    ct.exactMatching("y", 1);

    REQUIRE(ct.onMatcher().matchedPids.size() == 0);
  }

  SECTION("Multiple patterns") {
    dfc::Pid const firstPatternPid = 1, secondPatternPid = 2;
    SECTION("Can match multiple equal segments") {
      patterns->emplace_back(firstPatternPid, createPattern("x"));
      patterns->emplace_back(secondPatternPid, createPattern("x"));
      initializer.addPattern(0, patterns->at(0));
      initializer.addPattern(1, patterns->at(1));

      auto const ct = initializer.ct<TestOnMatcher>(patterns);
      ct.exactMatching("x", 1);

      REQUIRE(ct.onMatcher().matchedPids.size() == 2);
      REQUIRE(ct.onMatcher().matchedPids[0] == firstPatternPid);
      REQUIRE(ct.onMatcher().matchedPids[1] == secondPatternPid);
    }

    SECTION("Can match multiple different segments") {
      patterns->emplace_back(firstPatternPid, createPattern("x"));
      patterns->emplace_back(secondPatternPid, createPattern("y"));
      initializer.addPattern(0, patterns->at(0));
      initializer.addPattern(1, patterns->at(1));

      auto const ct = initializer.ct<TestOnMatcher>(patterns);
      ct.exactMatching("x", 1);
      ct.exactMatching("y", 1);

      REQUIRE(ct.onMatcher().matchedPids.size() == 2);
      REQUIRE(ct.onMatcher().matchedPids[0] == firstPatternPid);
      REQUIRE(ct.onMatcher().matchedPids[1] == secondPatternPid);
    }
  }
}
}  // namespace
