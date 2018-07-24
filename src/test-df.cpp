#include "catch.hpp"

#include "df.hpp"
#include "util-test.hpp"

using dfc::test::createCaseInsensitivePattern;
using dfc::test::twoBytePattern;

namespace {
TEST_CASE("Empty without patterns") {
  dfc::DirectFilter<dfc::PatternRange<1, 3>, uint16_t> df;

  int ors = 0;
  for (auto const byte : df.filter()) {
    ors |= byte;
  }

  REQUIRE(ors == 0);
}

TEST_CASE("Sets bit if pattern is within the size constraint") {
  dfc::DirectFilter<dfc::PatternRange<1, 3>, uint16_t> df;

  df.addPattern(twoBytePattern());

  REQUIRE(df.isSet(twoBytePattern().data()));
}

TEST_CASE("Does not set bit if pattern is outside the size constraint") {
  dfc::DirectFilter<dfc::PatternRange<3, 5>, uint16_t> df;

  df.addPattern(twoBytePattern());

  REQUIRE(!df.isSet(twoBytePattern().data()));
}

TEST_CASE("Sets bit for all permutations if pattern is case insensitive") {
  dfc::DirectFilter<dfc::PatternRange<1, 3>, uint16_t> df;

  df.addPattern(createCaseInsensitivePattern("ab"));

  REQUIRE(df.isSet("ab"));
  REQUIRE(df.isSet("Ab"));
  REQUIRE(df.isSet("aB"));
  REQUIRE(df.isSet("AB"));
}
}  // namespace
