#ifndef DFC_PATTERN_PARSER_HPP
#define DFC_PATTERN_PARSER_HPP

#include <optional>

#include "raw-pattern.hpp"

namespace dfc {
/**
 * PatternParser is used to convert a string into a pattern as represented by
 * this library. Currently simplified, but will be extended to support the
 * binary extension used by snort
 */
class PatternParser {
 public:
  /**
   * Returns a case sensitive RawPattern if the line contains a valid pattern,
   * std::nullopt otherwise
   */
  std::optional<RawPattern> parsePatternAsCaseSensitive(
      std::string const& line) const {
    if (line.empty()) {
      return std::nullopt;
    }

    return RawPattern(line, Pattern::CaseSensitivity::CaseSensitive);
  }

  /**
   * Returns a case insensitive RawPattern if the line contains a valid pattern,
   * std::nullopt otherwise
   */
  std::optional<RawPattern> parsePatternAsCaseInsensitive(
      std::string const& line) const {
    if (line.empty()) {
      return std::nullopt;
    }

    return RawPattern(line, Pattern::CaseSensitivity::CaseInsensitive);
  }
};
}  // namespace dfc

#endif
