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
    auto file = std::fopen(path_.c_str(), "rb");

    if (file == nullptr) {
      return DataFile(path_);
    }

    std::fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    std::fseek(file, 0, SEEK_SET);

    auto data = std::make_unique<byte[]>(size);

    size_t readCount = std::fread(data.get(), size, 1, file);
    std::fclose(file);

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

 public:
  explicit PatternFile(std::string path) : path_(std::move(path)) {}

  void readPatterns(std::function<void(dfc::RawPattern)> const& callback) const
      noexcept {
    std::ifstream input(path_);

    dfc::PatternParser patternParser;
    std::string line;
    while (std::getline(input, line)) {
      std::optional<dfc::RawPattern> maybePattern =
          patternParser.parsePatternAsCaseSensitive(line);

      if (maybePattern.has_value()) {
        callback(std::move(maybePattern.value()));
      }
    }
  }
};

bool isArgumentCountValid(int argc);
void printUsage(std::string const& programName);
CustomExecutionLoop parsePatternFileAndAddPatterns(
    std::string const& patternFilePath);
std::vector<dfc::ImmutablePattern> readFileAndAggregatePatterns(
    std::string const& patternFilePath);

DataFile readDataFile(std::string dataFilePath) {
  DataFile dataFile(std::move(dataFilePath));
  return dataFile.read();
}

std::vector<dfc::ImmutablePattern> aggregatePatterns(PatternFile patternFile) {
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

  CustomExecutionLoop const executionLoop =
      addPatternsToExecutionLoop(aggregatePatterns(PatternFile(argv[1])));

  dfc::SaveOnMatcher matcher;
  executionLoop.match(dfc::InputView(dataFile.data(), dataFile.size()),
                      matcher);

  std::cout << "Match count: " << matcher.matchedPids.size() << '\n';

  return 0;
}

bool isArgumentCountValid(int const argc) { return argc == 3; }

void printUsage(std::string const& programName) {
  std::cout << "Usage: " << programName << " pattern-file data-file\n";
}
