#include "catch.hpp"

#include "ct-initializer.hpp"
#include "df-initializer.hpp"
#include "match-path.hpp"
#include "util-test.hpp"

namespace {
using dfc::SaveOnMatcher;

auto createEmptyDf() {
  dfc::DirectFilterInitializer<dfc::PatternRange<2, 2>, uint16_t, 1, uint16_t>
      init;
  return init.df();
}
auto createEmptyCt() {
  dfc::CompactTableInitializer<dfc::PatternRange<2, 100>, uint16_t, 1, 100>
      init;
  return init.ct(nullptr);
}

auto createDfWithPattern(dfc::RawPattern pattern) {
  dfc::DirectFilterInitializer<dfc::PatternRange<2, 2>, uint16_t, 1, uint16_t>
      init;
  init.addPattern(pattern);

  return init.df();
}

auto createCtWithPattern(dfc::RawPattern pattern) {
  dfc::CompactTableInitializer<dfc::PatternRange<2, 100>, uint16_t, 1, 100>
      init;
  auto patterns = std::make_shared<std::vector<dfc::ImmutablePattern>>();
  patterns->emplace_back(0, std::move(pattern));
  init.addPattern(0, patterns->at(0));

  return init.ct(patterns);
}

using Df = decltype(createEmptyDf());
using Ct = decltype(createEmptyCt());

TEST_CASE("No match if input is not a pattern") {
  SaveOnMatcher onMatcher;
  dfc::MatchPath<Df, Ct> path(createEmptyDf(), createEmptyCt());

  std::string in("test");
  path.match(in.data(), in.size(), onMatcher);

  REQUIRE(onMatcher.matchedPids.empty());
}

TEST_CASE("Match if input equals pattern") {
  SaveOnMatcher onMatcher;
  std::string in("in");
  dfc::MatchPath<Df, Ct> path(
      createDfWithPattern(dfc::test::createPattern(in.data())),
      createCtWithPattern(dfc::test::createPattern(in.data())));

  path.match(in.data(), in.size(), onMatcher);

  REQUIRE(onMatcher.matchedPids.size() == 1);
}
}  // namespace
