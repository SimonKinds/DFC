#ifndef DFC_RAW_PATTERN_HPP
#define DFC_RAW_PATTERN_HPP

#include <cstring>
#include <memory>

#include "pattern.hpp"

namespace dfc {
class RawPattern final : public Pattern {
 private:
  int size_;
  CaseSensitivity caseSensitivity_;
  std::shared_ptr<byte[]> data_;

 public:
  explicit RawPattern(byte const* const data, int const size)
      : RawPattern(data, size, CaseSensitivity::CaseSensitive) {}

  explicit RawPattern(byte const* const data, int size,
                      CaseSensitivity caseSensitivity)
      : size_(size),
        caseSensitivity_(caseSensitivity),
        data_(std::make_unique<byte[]>(size)) {
    std::memcpy(data_.get(), data, size);
  }

  byte const* data() const noexcept override { return data_.get(); }
  int size() const noexcept override { return size_; }
  CaseSensitivity caseSensitivity() const noexcept override {
    return caseSensitivity_;
  }

  std::shared_ptr<byte[]> ptr() const { return data_; }
};
}  // namespace dfc

#endif
