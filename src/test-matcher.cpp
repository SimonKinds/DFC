#include "catch.hpp"

#include "matcher.hpp"
#include "util-test.hpp"

using dfc::ImmutablePattern;
using dfc::InputView;
using dfc::Pid;
using dfc::test::createCaseInsensitiveImmutablePattern;
using dfc::test::createCaseInsensitivePattern;
using dfc::test::fiveBytePattern;

namespace {
TEST_CASE("Matcher matches if equal") {
  dfc::Matcher matcher;

  ImmutablePattern pattern(Pid{0}, fiveBytePattern());
  InputView input(pattern.data(), pattern.size());

  REQUIRE(matcher.matches(input, pattern) == true);
}

TEST_CASE("Matcher does not match if not equal") {
  dfc::Matcher matcher;

  ImmutablePattern pattern(Pid{0}, fiveBytePattern());
  REQUIRE(matcher.matches(InputView("12345"), pattern) == false);
}

TEST_CASE("Matcher matches all variants of case insensitive patterns") {
  ImmutablePattern pattern(Pid{0}, createCaseInsensitivePattern("ab"));

  dfc::Matcher matcher;
  REQUIRE(matcher.matches(InputView("ab"), pattern));
  REQUIRE(matcher.matches(InputView("Ab"), pattern));
  REQUIRE(matcher.matches(InputView("aB"), pattern));
  REQUIRE(matcher.matches(InputView("AB"), pattern));
}

TEST_CASE("Matcher does not match if remaining bytes are fewer than pattern") {
  auto const pattern = createCaseInsensitiveImmutablePattern(Pid{0}, "ab");

  dfc::Matcher matcher;
  REQUIRE(matcher.matches(InputView("a"), pattern) == false);
}

}  // namespace
