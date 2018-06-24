#ifndef DFC_PATTERN_HPP
#define DFC_PATTERN_HPP

#include <cstring>
#include <memory>

#include "byte.hpp"
#include "pid.hpp"
#include "raw-pattern.hpp"

namespace dfc {
class Pattern {
 private:
  Pid const pid_;
  int const size_;
  std::unique_ptr<byte const[]> data_;

 public:
  Pattern(Pid const pid, RawPattern&& pattern)
      : pid_(pid), size_(pattern.size()), data_(pattern.giveOwnership()) {}

  Pattern(Pattern&&) = default;
  Pattern& operator=(Pattern&&) = default;

  Pattern(const Pattern&) = delete;
  Pattern& operator=(const Pattern&) = delete;

  inline Pid pid() const noexcept { return pid_; }
  inline byte const* data() const noexcept { return data_.get(); }
  inline int size() const noexcept { return size_; }

  bool operator==(RawPattern const& other) const {
    return size_ == other.size() &&
           memcmp(data_.get(), other.data(), size_) == 0;
  }
};

}  // namespace dfc

#endif
