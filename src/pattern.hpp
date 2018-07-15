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
  std::shared_ptr<byte[]> data_;
  int size_;

 public:
  explicit RawPattern(byte const* const data, int size)
      : data_(std::make_unique<byte[]>(size)), size_(size) {
    std::memcpy(data_.get(), data, size);
  }

  byte const* data() const noexcept override { return data_.get(); }
  int size() const noexcept override { return size_; }

  std::shared_ptr<byte[]> const& ptr() const { return data_; }
};

class ImmutablePattern final : public Pattern {
 private:
  Pid const pid_;
  int const size_;
  std::shared_ptr<byte const[]> const data_;

 public:
  explicit ImmutablePattern(Pid const pid, RawPattern const& pattern)
      : pid_(pid), size_(pattern.size()), data_(pattern.ptr()) {}

  inline Pid pid() const noexcept { return pid_; }
  inline byte const* data() const noexcept override { return data_.get(); }
  inline int size() const noexcept override { return size_; }
};

}  // namespace dfc

#endif
