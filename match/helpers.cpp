#include "helpers.hpp"

#include <iostream>

#include "pattern-aggregator.hpp"

namespace dfc {
namespace match {
bool isArgumentCountValid(int const argc) { return argc == 3 || argc == 4; }

void printUsage(std::string const& programName) {
  std::cout << "Usage: " << programName
            << " pattern-file data-file [case-sensitive=1|0]\n";
}

Pattern::CaseSensitivity caseSensitivityOfParsedPatterns(int const argc,
                                                         char** argv) {
  if (argc == 4 && std::strcmp(argv[3], "1") == 0) {
    return Pattern::CaseSensitivity::CaseInsensitive;
  }

  return Pattern::CaseSensitivity::CaseSensitive;
}

void printExecutionInformation(PatternFile const& patternFile,
                               DataFile const& dataFile) {
  std::cout << "Matching " << dataFile.size() << " bytes against "
            << (patternFile.readAsCaseSensitive() ? "case sensitive "
                                                  : "case insensitive ")
            << "patterns\n"
            << std::endl;
}
std::vector<ImmutablePattern> readFileAndAggregatePatterns(
    std::string const& patternFilePath);

DataFile readDataFile(std::string dataFilePath) {
  DataFile dataFile(std::move(dataFilePath));
  return dataFile.read();
}

int countMatches(CustomExecutionLoop const& executionLoop,
                 DataFile const& dataFile) {
  CountOnMatcher matcher;
  executionLoop.match(InputView(dataFile.data(), dataFile.size()), matcher);

  return matcher.matchCount;
}

void printExecutionInformation(PatternFile const& patternFile,
                               DataFile const& dataFile);

std::vector<ImmutablePattern> aggregatePatterns(
    PatternFile const& patternFile) {
  PatternAggregator patternAggregator;
  patternFile.readPatterns([&patternAggregator](RawPattern pattern) {
    patternAggregator.add(std::move(pattern));
  });

  return patternAggregator.aggregate();
}

CustomExecutionLoop addPatternsToExecutionLoop(
    std::vector<ImmutablePattern> patterns) {
  CustomExecutionLoop executionLoop;
  std::for_each(std::begin(patterns), std::end(patterns),
                [&executionLoop](ImmutablePattern const& pattern) {
                  executionLoop.addPattern(pattern);
                });

  return executionLoop;
}

}  // namespace match
}  // namespace dfc
