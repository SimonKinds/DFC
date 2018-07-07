#include "catch.hpp"

#include "matcher.hpp"
#include "util-test.hpp"

using dfc::Pattern;
using dfc::test::fiveBytePattern;

namespace {
TEST_CASE("Loop matcher matches if equal") {
  dfc::LoopMatcher matcher;

  Pattern pattern(0, fiveBytePattern());
  REQUIRE(matcher.matches(pattern.data(), pattern.size(), pattern) == true);
}

TEST_CASE("Loop matcher does not match if not equal") {
  dfc::LoopMatcher matcher;

  Pattern pattern(0, fiveBytePattern());
  REQUIRE(matcher.matches("12345", pattern.size(), pattern) == false);
}

TEST_CASE("Memcmp matcher matches if equal") {
  dfc::MemcmpMatcher matcher;

  Pattern pattern(0, fiveBytePattern());
  REQUIRE(matcher.matches(pattern.data(), pattern.size(), pattern) == true);
}

TEST_CASE("Memcmp matcher does not match if not equal") {
  dfc::MemcmpMatcher matcher;

  Pattern pattern(0, fiveBytePattern());
  REQUIRE(matcher.matches("12345", pattern.size(), pattern) == false);
}
}  // namespace
