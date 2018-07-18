#ifndef DFC_IMMUTABLE_PATTERN_HPP
#define DFC_IMMUTABLE_PATTERN_HPP

#include "pid.hpp"
#include "raw-pattern.hpp"

namespace dfc {
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
  inline bool caseSensitive() const noexcept override {
    return caseSensitive_;
  };
};
}  // namespace dfc

#endif
