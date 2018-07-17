#include "catch.hpp"

#include "matcher.hpp"
#include "util-test.hpp"

using dfc::ImmutablePattern;
using dfc::test::createCaseInsensitivePattern;
using dfc::test::fiveBytePattern;

namespace {
TEST_CASE("Matcher matches if equal") {
  dfc::Matcher matcher;

  ImmutablePattern pattern(0, fiveBytePattern());
  REQUIRE(matcher.matches(pattern.data(), pattern.size(), pattern) == true);
}

TEST_CASE("Matcher does not match if not equal") {
  dfc::Matcher matcher;

  ImmutablePattern pattern(0, fiveBytePattern());
  REQUIRE(matcher.matches("12345", pattern.size(), pattern) == false);
}

TEST_CASE("Matcher matches all variants of case insensitive patterns") {
  ImmutablePattern pattern(0, createCaseInsensitivePattern("ab"));

  dfc::Matcher matcher;
  REQUIRE(matcher.matches("ab", 2, pattern));
  REQUIRE(matcher.matches("Ab", 2, pattern));
  REQUIRE(matcher.matches("aB", 2, pattern));
  REQUIRE(matcher.matches("AB", 2, pattern));
}

}  // namespace
