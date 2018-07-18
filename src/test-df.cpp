#include "catch.hpp"

#include "df-initializer.hpp"
#include "indexer.hpp"
#include "segmenter.hpp"
#include "util-test.hpp"

using dfc::test::createCaseInsensitivePattern;
using dfc::test::twoBytePattern;

namespace {
TEST_CASE("Empty without patterns") {
  dfc::DirectFilterInitializer<dfc::PatternRange<1, 3>, uint16_t> init;

  auto df = init.df();

  int ors = 0;
  for (auto const byte : df.filter()) {
    ors |= byte;
  }

  REQUIRE(ors == 0);
}

TEST_CASE("Sets bit if pattern is within the size constraint") {
  dfc::DirectFilterInitializer<dfc::PatternRange<1, 3>, uint16_t> init;

  init.addPattern(twoBytePattern());

  auto const df = init.df();
  REQUIRE(df.isSet(twoBytePattern().data()));
}

TEST_CASE("Does not set bit if pattern is outside the size constraint") {
  dfc::DirectFilterInitializer<dfc::PatternRange<3, 5>, uint16_t> init;

  init.addPattern(twoBytePattern());

  auto const df = init.df();
  REQUIRE(!df.isSet(twoBytePattern().data()));
}

TEST_CASE("Sets bit for all permutations if pattern is case insensitive") {
  dfc::DirectFilterInitializer<dfc::PatternRange<1, 3>, uint16_t> init;

  init.addPattern(createCaseInsensitivePattern("ab"));

  auto const df = init.df();
  REQUIRE(df.isSet("ab"));
  REQUIRE(df.isSet("Ab"));
  REQUIRE(df.isSet("aB"));
  REQUIRE(df.isSet("AB"));
}
}  // namespace
