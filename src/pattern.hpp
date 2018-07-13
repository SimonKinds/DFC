#ifndef DFC_PATTERN_HPP
#define DFC_PATTERN_HPP

#include <cstring>
#include <memory>

#include "byte.hpp"
#include "pid.hpp"

namespace dfc {
class Pattern {
 public:
  virtual ~Pattern() = default;

  virtual byte const* data() const noexcept = 0;
  virtual int size() const noexcept = 0;

  bool operator==(Pattern const& other) const {
    return size() == other.size() && memcmp(data(), other.data(), size()) == 0;
  }

  bool operator<(Pattern const& other) const {
    if (size() != other.size()) {
      return size() < other.size();
    }

    return memcmp(data(), other.data(), size()) == -1;
  }

  bool operator>(Pattern const& other) const {
    if (size() != other.size()) {
      return size() > other.size();
    }

    return memcmp(data(), other.data(), size()) == 1;
  }
};

class RawPattern final : public Pattern {
 private:
  using DataPtr = std::unique_ptr<byte[]>;
  DataPtr data_;
  int size_;

 public:
  explicit RawPattern(byte const* const data, int size)
      : data_(std::make_unique<byte[]>(size)), size_(size) {
    std::memcpy(data_.get(), data, size);
  }

  RawPattern& operator=(RawPattern&& other) {
    size_ = other.size();
    data_ = other.giveOwnership();

    return *this;
  }

  RawPattern(RawPattern const&) = delete;
  RawPattern(RawPattern&&) = default;

  byte const* data() const noexcept override { return data_.get(); }
  int size() const noexcept override { return size_; }

  DataPtr&& giveOwnership() { return std::move(data_); }
};

class ImmutablePattern final : public Pattern {
 private:
  Pid const pid_;
  int const size_;
  std::unique_ptr<byte const[]> data_;

 public:
  ImmutablePattern(Pid const pid, RawPattern&& pattern)
      : pid_(pid), size_(pattern.size()), data_(pattern.giveOwnership()) {}

  ImmutablePattern(ImmutablePattern&&) = default;
  ImmutablePattern& operator=(ImmutablePattern&&) = default;

  ImmutablePattern(const ImmutablePattern&) = delete;
  ImmutablePattern& operator=(const ImmutablePattern&) = delete;

  inline Pid pid() const noexcept { return pid_; }
  inline byte const* data() const noexcept override { return data_.get(); }
  inline int size() const noexcept override { return size_; }
};

}  // namespace dfc

#endif
