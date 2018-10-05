#ifndef DFC_MATCH_PATTERN_FILE_HPP
#define DFC_MATCH_PATTERN_FILE_HPP

#include <fstream>
#include <functional>
#include <string>

#include "pattern-parser.hpp"
#include "raw-pattern.hpp"

namespace dfc {
namespace match {

class PatternFile {
  std::string path_;
  Pattern::CaseSensitivity caseSensitivity_;

 public:
  explicit PatternFile(std::string path,
                       dfc::Pattern::CaseSensitivity caseSensitivity)
      : path_(std::move(path)), caseSensitivity_(caseSensitivity) {}

  void readPatterns(std::function<void(dfc::RawPattern)> const& callback) const
      noexcept {
    std::ifstream input(path_);

    std::string line;
    while (std::getline(input, line)) {
      std::optional<dfc::RawPattern> maybePattern = readPattern(line);

      if (maybePattern.has_value()) {
        callback(std::move(maybePattern.value()));
      }
    }
  }

  bool readAsCaseSensitive() const noexcept {
    return caseSensitivity_ == dfc::Pattern::CaseSensitivity::CaseSensitive;
  }

 private:
  std::optional<dfc::RawPattern> readPattern(std::string const& line) const
      noexcept {
    dfc::PatternParser const patternParser;
    if (readAsCaseSensitive()) {
      return patternParser.parsePatternAsCaseSensitive(line);
    }

    return patternParser.parsePatternAsCaseInsensitive(line);
  }
};

}  // namespace match
}  // namespace dfc

#endif
