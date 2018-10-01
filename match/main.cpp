#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>

#include "byte.hpp"
#include "execution-loop.hpp"
#include "layered-direct-filter.hpp"
#include "match-path.hpp"
#include "pattern-aggregator.hpp"
#include "pattern-parser.hpp"

using TwoByteDirectFilter = dfc::FlatDirectFilter<uint16_t>;
using OneByteCompactTable = dfc::CompactTable<uint8_t, 1, 0x100>;

using FourByteDirectFilterWithMask =
    dfc::FlatDirectFilter<uint32_t, 8387, uint16_t>;
using FourByteCompactTable = dfc::CompactTable<uint32_t, 8389, 0x20000>;

using CustomExecutionLoop = dfc::ExecutionLoop<
    dfc::MatchPath<dfc::PatternRange<1, 3>, TwoByteDirectFilter,
                   OneByteCompactTable>,
    dfc::MatchPath<dfc::PatternRange<4, 64>,
                   dfc::LayeredDirectFilter<TwoByteDirectFilter,
                                            FourByteDirectFilterWithMask>,
                   FourByteCompactTable>>;

class DataFile {
  std::string path_;
  std::unique_ptr<byte[]> data_;
  int64_t size_;

 public:
  explicit DataFile(std::string path) : path_(std::move(path)) {}
  DataFile(std::string path, std::unique_ptr<byte[]> data, int64_t const size)
      : path_(std::move(path)), data_(std::move(data)), size_(size) {}

  DataFile read() const noexcept {
    std::unique_ptr<std::FILE, std::function<void(std::FILE*)>> file(
        std::fopen(path_.c_str(), "rb"), [](FILE* file) { std::fclose(file); });

    if (file == nullptr) {
      return DataFile(path_);
    }

    std::fseek(file.get(), 0, SEEK_END);
    size_t size = ftell(file.get());
    std::fseek(file.get(), 0, SEEK_SET);

    auto data = std::make_unique<byte[]>(size);

    size_t readCount = std::fread(data.get(), size, 1, file.get());

    if (readCount != 1) {
      std::perror("Could not read file");
      return DataFile(path_);
    }

    return DataFile(path_, std::move(data), static_cast<int64_t>(size));
  }

  byte const* data() const noexcept { return data_.get(); }
  int64_t size() const noexcept { return size_; }
};

class PatternFile {
  std::string path_;
  dfc::Pattern::CaseSensitivity caseSensitivity_;

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

bool isArgumentCountValid(int argc);
void printUsage(std::string const& programName);
dfc::Pattern::CaseSensitivity caseSensitivityOfParsedPatterns(int argc,
                                                              char** argv);
std::vector<dfc::ImmutablePattern> readFileAndAggregatePatterns(
    std::string const& patternFilePath);

DataFile readDataFile(std::string dataFilePath) {
  DataFile dataFile(std::move(dataFilePath));
  return dataFile.read();
}

int countMatches(CustomExecutionLoop const& executionLoop,
                 DataFile const& dataFile) {
  dfc::CountOnMatcher matcher;
  executionLoop.match(dfc::InputView(dataFile.data(), dataFile.size()),
                      matcher);

  return matcher.matchCount;
}

void printExecutionInformation(PatternFile const& patternFile,
                               DataFile const& dataFile);

std::vector<dfc::ImmutablePattern> aggregatePatterns(
    PatternFile const& patternFile) {
  dfc::PatternAggregator patternAggregator;
  patternFile.readPatterns([&patternAggregator](dfc::RawPattern pattern) {
    patternAggregator.add(std::move(pattern));
  });

  return patternAggregator.aggregate();
}

CustomExecutionLoop addPatternsToExecutionLoop(
    std::vector<dfc::ImmutablePattern> patterns) {
  CustomExecutionLoop executionLoop;
  std::for_each(std::begin(patterns), std::end(patterns),
                [&executionLoop](dfc::ImmutablePattern const& pattern) {
                  executionLoop.addPattern(pattern);
                });

  return executionLoop;
}

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

bool isArgumentCountValid(int const argc) { return argc == 3 || argc == 4; }

void printUsage(std::string const& programName) {
  std::cout << "Usage: " << programName
            << " pattern-file data-file [case-sensitive=1|0]\n";
}

dfc::Pattern::CaseSensitivity caseSensitivityOfParsedPatterns(int const argc,
                                                              char** argv) {
  if (argc == 4 && std::strcmp(argv[3], "1") == 0) {
    return dfc::Pattern::CaseSensitivity::CaseInsensitive;
  }

  return dfc::Pattern::CaseSensitivity::CaseSensitive;
}

void printExecutionInformation(PatternFile const& patternFile,
                               DataFile const& dataFile) {
  std::cout << "Matching " << dataFile.size() << " bytes against "
            << (patternFile.readAsCaseSensitive() ? "case sensitive "
                                                  : "case insensitive ")
            << "patterns\n"
            << std::endl;
}
