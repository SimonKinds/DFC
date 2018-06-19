#include "catch.hpp"

#include "pattern-aggregator.hpp"

using dfc::PatternAggregator;
using dfc::RawPattern;

RawPattern testPattern() {
  std::string string_pat("test");
  RawPattern pat(reinterpret_cast<std::byte*>(string_pat.data()),
                 string_pat.size());

  return pat;
}

RawPattern testPattern2() {
  std::string string_pat("another one");
  RawPattern pat(reinterpret_cast<std::byte*>(string_pat.data()),
                 string_pat.size());

  return pat;
}

TEST_CASE("emptyByDefault") {
  PatternAggregator agg;

  REQUIRE(agg.aggregate().empty());
}

TEST_CASE("patternIsAdded") {
  PatternAggregator agg;
  agg.add(testPattern());

  const auto& vec = agg.aggregate();

  REQUIRE(vec.size() == 1);
  REQUIRE(vec[0] == testPattern());
}

TEST_CASE("duplicatesAreRemoved") {
  PatternAggregator agg;
  agg.add(testPattern());
  agg.add(testPattern());

  const auto& vec = agg.aggregate();

  REQUIRE(vec.size() == 1);
  REQUIRE(vec[0] == testPattern());
}

TEST_CASE("canAddMultiple") {
  PatternAggregator agg;
  agg.add(testPattern());
  agg.add(testPattern2());

  const auto& vec = agg.aggregate();

  REQUIRE(vec.size() == 2);
}
