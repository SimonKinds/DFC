#ifndef PATTERN_AGGREGATOR_H
#define PATTERN_AGGREGATOR_H

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <functional>
#include <memory>
#include <vector>

namespace dfc {
class RawPattern {
 private:
  static void deleter(std::byte* ptr) { free(ptr); }

  using DataPtr = std::unique_ptr<std::byte, std::function<void(std::byte*)>>;
  DataPtr data_;
  int size_;

 public:
  RawPattern(const std::byte* const data, int size)
      : data_(static_cast<std::byte*>(malloc(size)), deleter), size_(size) {
    std::memcpy(data_.get(), data, size);
  }

  RawPattern& operator=(RawPattern&& other) {
    size_ = other.size();
    data_ = other.takeOwnership();

    return *this;
  }

  RawPattern(const RawPattern&) = delete;
  RawPattern(RawPattern&&) = default;

  bool operator==(const RawPattern& other) const {
    return size_ == other.size() &&
           memcmp(data_.get(), other.data(), size_) == 0;
  }

  bool operator<(const RawPattern& other) const {
    if (size_ != other.size()) {
      return size_ < other.size();
    }

    return memcmp(data_.get(), other.data(), size_) == -1;
  }

  bool operator>(const RawPattern& other) const {
    if (size_ != other.size()) {
      return size_ > other.size();
    }

    return memcmp(data_.get(), other.data(), size_) == 1;
  }

  const std::byte* data() const { return data_.get(); }

  int size() const { return size_; }

  DataPtr takeOwnership() { return std::move(data_); }
};

class PatternAggregator {
 private:
  std::vector<RawPattern> patterns_;

 public:
  void add(RawPattern pat) { patterns_.emplace_back(std::move(pat)); }

  const std::vector<RawPattern>& aggregate() {
    removeDuplicates();

    return patterns_;
  }

 private:
  void removeDuplicates() {
    std::sort(std::begin(patterns_), std::end(patterns_));
    patterns_.erase(std::unique(std::begin(patterns_), std::end(patterns_)),
                    std::end(patterns_));
  }
};
}  // namespace dfc

#endif
