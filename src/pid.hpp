#ifndef DFC_PID_HPP
#define DFC_PID_HPP

namespace dfc {
struct Pid {
  int pid_;

  explicit Pid(int const pid) : pid_(pid) {}

  int value() const noexcept { return pid_; }
  Pid next() const { return Pid{pid_ + 1}; }

  bool operator==(Pid const& other) const noexcept {
    return this->pid_ == other.pid_;
  }
};

struct PidIndex {
  int index_;

  explicit PidIndex(int const index) : index_(index) {}

  int value() const noexcept { return index_; }

  bool operator==(PidIndex const& other) const noexcept {
    return this->index_ == other.index_;
  }
};

}  // namespace dfc

#endif
