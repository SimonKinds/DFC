#ifndef DFC_MATCH_HELPERS_HPP
#define DFC_MATCH_HELPERS_HPP

#include <vector>

#include "config.hpp"
#include "data-file.hpp"
#include "pattern-file.hpp"

namespace dfc {
namespace match {
bool isArgumentCountValid(int argc);
void printUsage(std::string const& programName);
Pattern::CaseSensitivity caseSensitivityOfParsedPatterns(int argc, char** argv);
std::vector<ImmutablePattern> readFileAndAggregatePatterns(
    std::string const& patternFilePath);

DataFile readDataFile(std::string dataFilePath);

int countMatches(CustomExecutionLoop const& executionLoop,
                 DataFile const& dataFile);

void printExecutionInformation(PatternFile const& patternFile,
                               DataFile const& dataFile);

std::vector<ImmutablePattern> aggregatePatterns(PatternFile const& patternFile);

CustomExecutionLoop addPatternsToExecutionLoop(
    std::vector<ImmutablePattern> patterns);

}  // namespace match
}  // namespace dfc

#endif
