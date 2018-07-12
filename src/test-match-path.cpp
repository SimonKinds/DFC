#include "catch.hpp"

#include "ct-initializer.hpp"
#include "df-initializer.hpp"
#include "match-path.hpp"
#include "util-test.hpp"

namespace {
auto createEmptyDf() {
  dfc::DirectFilterInitializer<dfc::PatternRange<2, 2>, uint16_t, 1, uint16_t>
      init;
  return init.df();
}
auto createEmptyCt(std::shared_ptr<dfc::SaveOnMatcher> onMatcher) {
  dfc::CompactTableInitializer<dfc::PatternRange<1, 100>, uint16_t, 1, 100>
      init;
  return init.ct<dfc::SaveOnMatcher>(std::move(onMatcher), nullptr);
}

auto createDfWithPattern(dfc::RawPattern pattern) {
  dfc::DirectFilterInitializer<dfc::PatternRange<2, 2>, uint16_t, 1, uint16_t>
      init;
  init.addPattern(pattern);

  return init.df();
}

auto createCtWithPattern(std::shared_ptr<dfc::SaveOnMatcher> onMatcher,
                         dfc::RawPattern pattern) {
  dfc::CompactTableInitializer<dfc::PatternRange<1, 100>, uint16_t, 1, 100>
      init;
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();
  patterns->emplace_back(0, std::move(pattern));
  init.addPattern(0, patterns->at(0));

  return init.ct(std::move(onMatcher), patterns);
}

using Df = decltype(createEmptyDf());
using Ct = decltype(createEmptyCt(nullptr));

TEST_CASE("No match if input is not a pattern") {
  auto onMatcher = std::make_shared<dfc::SaveOnMatcher>();
  dfc::MatchPath<Df, Ct> path(createEmptyDf(), createEmptyCt(onMatcher));

  std::string in("test");
  path.match(in.data(), in.size());

  REQUIRE(onMatcher->matchedPids.empty());
}

TEST_CASE("Match if input equals pattern") {
  auto onMatcher = std::make_shared<dfc::SaveOnMatcher>();
  std::string in("in");
  dfc::MatchPath<Df, Ct> path(
      createDfWithPattern(dfc::test::createPattern(in.data())),
      createCtWithPattern(onMatcher, dfc::test::createPattern(in.data())));

  path.match(in.data(), in.size());

  REQUIRE(onMatcher->matchedPids.size() == 1);
}
}  // namespace
