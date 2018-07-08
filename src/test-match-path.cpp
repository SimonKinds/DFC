#include "catch.hpp"

#include "ct-initializer.hpp"
#include "df-initializer.hpp"
#include "match-path.hpp"
#include "util-test.hpp"

namespace {
auto createEmptyDf() {
  dfc::DirectFilterInitializer<uint16_t, 1, uint16_t> init(2, 2);
  return init.df();
}
auto createEmptyCt() {
  dfc::CompactTableInitializer<uint16_t, 1, 100> init;
  return init.ct<dfc::SaveOnMatcher>(nullptr);
}

auto createDfWithPattern(dfc::RawPattern pattern) {
  dfc::DirectFilterInitializer<uint16_t, 1, uint16_t> init(2, 2);
  init.addPattern(pattern);

  return init.df();
}

auto createCtWithPattern(dfc::RawPattern pattern) {
  dfc::CompactTableInitializer<uint16_t, 1, 100> init;
  auto patterns = std::make_shared<std::vector<dfc::Pattern>>();
  patterns->emplace_back(0, std::move(pattern));
  init.addPattern(0, patterns->at(0));

  return init.ct<dfc::SaveOnMatcher>(patterns);
}

using Df = decltype(createEmptyDf());
using Ct = decltype(createEmptyCt());

template <typename... T>
std::vector<dfc::Pid> getMatchedPids(dfc::MatchPath<T...> const& path) {
  return path.ct().onMatcher().matchedPids;
}

TEST_CASE("No match if input is not a pattern") {
  dfc::MatchPath<Df, Ct> path(createEmptyDf(), createEmptyCt());

  std::string in("test");
  path.match(in.data(), in.size());

  REQUIRE(getMatchedPids(path).empty());
}

TEST_CASE("Match if input equals pattern") {
  std::string in("in");
  dfc::MatchPath<Df, Ct> path(
      createDfWithPattern(dfc::test::createPattern(in.data())),
      createCtWithPattern(dfc::test::createPattern(in.data())));

  path.match(in.data(), in.size());

  REQUIRE(getMatchedPids(path).size() == 1);
}
}  // namespace
