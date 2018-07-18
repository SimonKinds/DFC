#ifndef DFC_RAW_PATTERN_HPP
#define DFC_RAW_PATTERN_HPP

#include <cstring>
#include <memory>

#include "pattern.hpp"

namespace dfc {
class RawPattern final : public Pattern {
 private:
  int size_;
  bool caseSensitive_;
  std::shared_ptr<byte[]> data_;

 public:
  explicit RawPattern(byte const* const data, int const size)
      : RawPattern(data, size, true) {}

  explicit RawPattern(byte const* const data, int size, int caseSensitive)
      : size_(size),
        caseSensitive_(caseSensitive),
        data_(std::make_unique<byte[]>(size)) {
    std::memcpy(data_.get(), data, size);
  }

  byte const* data() const noexcept override { return data_.get(); }
  int size() const noexcept override { return size_; }
  bool caseSensitive() const noexcept override { return caseSensitive_; };

  std::shared_ptr<byte[]> ptr() const { return data_; }
};
}  // namespace dfc

#endif
