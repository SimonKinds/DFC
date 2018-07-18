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
  virtual bool caseSensitive() const noexcept = 0;

  virtual bool operator==(Pattern const& other) const;
  virtual bool operator<(Pattern const& other) const;
  virtual bool operator>(Pattern const& other) const;
};

class RawPattern final : public Pattern {
 private:
  std::shared_ptr<byte[]> data_;
  int size_;
  bool caseSensitive_;

 public:
  explicit RawPattern(byte const* const data, int const size)
      : RawPattern(data, size, true) {}

  explicit RawPattern(byte const* const data, int size, int caseSensitive)
      : data_(std::make_unique<byte[]>(size)),
        size_(size),
        caseSensitive_(caseSensitive) {
    std::memcpy(data_.get(), data, size);
  }

  byte const* data() const noexcept override { return data_.get(); }
  int size() const noexcept override { return size_; }
  bool caseSensitive() const noexcept override { return caseSensitive_; };

  std::shared_ptr<byte[]> ptr() const { return data_; }
};

class ImmutablePattern final : public Pattern {
 private:
  Pid const pid_;
  int const size_;
  bool const caseSensitive_;
  std::shared_ptr<byte const[]> const data_;

 public:
  explicit ImmutablePattern(Pid const pid, RawPattern const& pattern)
      : pid_(pid),
        size_(pattern.size()),
        caseSensitive_(pattern.caseSensitive()),
        data_(pattern.ptr()) {}

  inline Pid pid() const noexcept { return pid_; }
  inline byte const* data() const noexcept override { return data_.get(); }
  inline int size() const noexcept override { return size_; }
  bool caseSensitive() const noexcept override { return caseSensitive_; };
};

}  // namespace dfc

#endif
