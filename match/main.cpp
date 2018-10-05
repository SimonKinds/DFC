#include <chrono>
#include <iostream>

#include "helpers.hpp"

using namespace dfc::match;

int main(int argc, char** argv) {
  if (!isArgumentCountValid(argc)) {
    printUsage(argv[0]);
    return 1;
  }

  DataFile const dataFile = readDataFile(argv[2]);

  PatternFile const patternFile(argv[1],
                                caseSensitivityOfParsedPatterns(argc, argv));
  CustomExecutionLoop const executionLoop =
      addPatternsToExecutionLoop(aggregatePatterns(patternFile));

  printExecutionInformation(patternFile, dataFile);

  auto start = std::chrono::high_resolution_clock::now();
  auto matchCount = countMatches(executionLoop, dataFile);
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "Execution time in ms: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << std::endl;

  std::cout << "Match count: " << matchCount << '\n';

  return 0;
}
