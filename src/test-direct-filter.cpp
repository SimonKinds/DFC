#include "catch.hpp"

#include "direct-filter.hpp"
#include "util-test.hpp"

using dfc::test::createCaseInsensitivePattern;
using dfc::test::createPattern;
using dfc::test::twoBytePattern;

namespace {
TEST_CASE("Empty without patterns") {
  dfc::DirectFilter<uint16_t> df;

  int ors = 0;
  for (auto const byte : df.filter()) {
    ors |= byte;
  }

  REQUIRE(ors == 0);
}

TEST_CASE("Sets bit if pattern is within the size constraint") {
  dfc::DirectFilter<uint16_t> df;

  df.addPattern(twoBytePattern());

  REQUIRE(df.contains(twoBytePattern().data()));
}

TEST_CASE("Sets bit for all permutations if pattern is case insensitive") {
  dfc::DirectFilter<uint16_t> df;

  df.addPattern(createCaseInsensitivePattern("ab"));

  REQUIRE(df.contains("ab"));
  REQUIRE(df.contains("Ab"));
  REQUIRE(df.contains("aB"));
  REQUIRE(df.contains("AB"));
}

TEST_CASE("Extends pattern to all permutations of segment size if smaller") {
  dfc::DirectFilter<uint16_t> df;

  auto const pattern = "a";

  df.addPattern(createPattern(pattern));

  int matches = 0;
  for (int i = 0; i < 256; ++i) {
    std::string input(pattern);
    input += static_cast<char>(i);

    if (df.contains(input.c_str())) {
      ++matches;
    }
  }

  REQUIRE(matches == 256);
}

TEST_CASE("Only allows extending segment by one byte") {
  dfc::DirectFilter<uint32_t> df;

  REQUIRE_THROWS_AS(
      df.addPattern(createPattern("aa")),
      dfc::SegmentExtender<uint32_t>::TooManyPermutationsException);
}

}  // namespace
