#include "catch.hpp"

#include <cstring>

#include "pattern-parser.hpp"

namespace {

TEST_CASE("PatternParser can parse simple pattern") {
  std::string line("abc");
  dfc::PatternParser parser;
  std::optional<dfc::RawPattern> maybePattern =
      parser.parsePatternAsCaseSensitive(line);

  REQUIRE(maybePattern.has_value() == true);

  auto pattern = maybePattern.value();

  REQUIRE(pattern ==
          dfc::RawPattern(line, dfc::Pattern::CaseSensitivity::CaseSensitive));
}

TEST_CASE("PatternParser returns no pattern if empty line") {
  dfc::PatternParser parser;
  std::optional<dfc::RawPattern> maybePattern =
      parser.parsePatternAsCaseSensitive("");

  REQUIRE(maybePattern.has_value() == false);
}

TEST_CASE("PatternParser does not stop at null character") {
  std::string line("abc\0abc", 7);
  dfc::PatternParser parser;
  std::optional<dfc::RawPattern> maybePattern =
      parser.parsePatternAsCaseSensitive(line);

  REQUIRE(maybePattern.has_value() == true);

  auto pattern = maybePattern.value();

  REQUIRE(pattern.size() == 7);
  REQUIRE(pattern == dfc::RawPattern(line));
}

TEST_CASE("PatternParser can parse simple pattern as case insensitive") {
  std::string line("abc");
  dfc::PatternParser parser;
  std::optional<dfc::RawPattern> maybePattern =
      parser.parsePatternAsCaseInsensitive(line);

  REQUIRE(maybePattern.has_value() == true);

  auto pattern = maybePattern.value();

  REQUIRE(pattern == dfc::RawPattern(
                         line, dfc::Pattern::CaseSensitivity::CaseInsensitive));
}
}  // namespace
