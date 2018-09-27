#ifndef DFC_IMMUTABLE_PATTERN_HPP
#define DFC_IMMUTABLE_PATTERN_HPP

#include "pid.hpp"
#include "raw-pattern.hpp"

namespace dfc {
class ImmutablePattern final : public Pattern {
 private:
  Pid const pid_;
  int const size_;
  CaseSensitivity const caseSensitivity_;
  std::shared_ptr<byte const[]> const data_;

 public:
  explicit ImmutablePattern(Pid const pid, RawPattern const& pattern)
      : pid_(pid),
        size_(pattern.size()),
        caseSensitivity_(pattern.caseSensitivity()),
        data_(pattern.ptr()) {}

  inline Pid pid() const noexcept { return pid_; }
  inline byte const* data() const noexcept override { return data_.get(); }
  inline int size() const noexcept override { return size_; }
  inline CaseSensitivity caseSensitivity() const noexcept override {
    return caseSensitivity_;
  };
};
}  // namespace dfc

#endif
