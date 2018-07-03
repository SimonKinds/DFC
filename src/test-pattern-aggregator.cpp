#include "catch.hpp"

#include "pattern-aggregator.hpp"
#include "util-test.hpp"

using dfc::PatternAggregator;
using dfc::RawPattern;
using dfc::test::fiveBytePattern;
using dfc::test::twoBytePattern;

TEST_CASE("emptyByDefault") {
  PatternAggregator agg;

  REQUIRE(agg.aggregate().empty());
}

TEST_CASE("patternIsAdded") {
  PatternAggregator agg;
  agg.add(twoBytePattern());

  const auto& vec = agg.aggregate();

  REQUIRE(vec.size() == 1);
  REQUIRE(vec[0] == twoBytePattern());
}

TEST_CASE("duplicatesAreRemoved") {
  PatternAggregator agg;
  agg.add(twoBytePattern());
  agg.add(twoBytePattern());

  const auto& vec = agg.aggregate();

  REQUIRE(vec.size() == 1);
  REQUIRE(vec[0] == twoBytePattern());
}

TEST_CASE("canAddMultiple") {
  PatternAggregator agg;
  agg.add(twoBytePattern());
  agg.add(fiveBytePattern());

  const auto& vec = agg.aggregate();

  REQUIRE(vec.size() == 2);
}
