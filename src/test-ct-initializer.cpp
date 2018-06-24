#include "catch.hpp"

#include "ct-initializer.hpp"

namespace {
std::vector<dfc::Pid> matchedPids;
struct TestOnMatcher : public dfc::OnMatcher<TestOnMatcher> {
  void onMatch(dfc::Pattern const& pattern) const {
    matchedPids.emplace_back(pattern.pid());
  }
};

dfc::RawPattern oneBytePattern() {
  std::string string_pat("t");
  dfc::RawPattern pat(reinterpret_cast<byte const*>(string_pat.data()),
                      string_pat.size());

  return pat;
}

TEST_CASE("CT") {
  matchedPids.clear();

  SECTION("1B CT") {
    auto patterns = std::make_shared<std::vector<dfc::Pattern>>();

    int const ctSize = 0x100;
    dfc::CompactTableInitializer<uint8_t, 1, ctSize> initializer;

    SECTION("Is empty by default") {
      auto const ct = initializer.ct<TestOnMatcher>(patterns);

      for (int i = 0; i < ctSize; ++i) {
        byte in = i;
        ct.exactMatching(&in, 1);
      }

      REQUIRE(matchedPids.size() == 0);
    }

    SECTION("Matches if added and equal") {
      dfc::Pid const pid = 1;
      patterns->emplace_back(pid, oneBytePattern());
      int const patternIndex = 0;
      initializer.addPattern(patternIndex, patterns->at(patternIndex));

      auto const ct = initializer.ct<TestOnMatcher>(patterns);
      ct.exactMatching(oneBytePattern().data(), oneBytePattern().size());

      REQUIRE(matchedPids.size() == 1);
      REQUIRE(matchedPids[0] == pid);
    }
  }
}
}  // namespace
