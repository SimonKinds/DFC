#ifndef DFC_PATTERN_HPP
#define DFC_PATTERN_HPP

#include <cstring>
#include <memory>

#include "byte.hpp"

namespace dfc {
class RawPattern {
 private:
  using DataPtr = std::unique_ptr<byte[]>;
  DataPtr data_;
  int size_;

 public:
  RawPattern(const byte* const data, int size)
      : data_(std::make_unique<byte[]>(size)), size_(size) {
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

  const byte* data() const { return data_.get(); }

  int size() const { return size_; }

  DataPtr takeOwnership() { return std::move(data_); }
};

}  // namespace dfc

#endif
