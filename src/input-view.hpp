#ifndef DFC_INPUT_VIEW_HPP
#define DFC_INPUT_VIEW_HPP

#include <cstdint>
#include <cstring>

#include "byte.hpp"

namespace dfc {
// A non-owning view into the input, much like a string_view
class InputView {
  byte const *const data_;
  int const size_;

 public:
  explicit InputView(char const *const data)
      : InputView(data, std::strlen(data)) {}

  InputView(char const *const data, int const size)
      : InputView(reinterpret_cast<byte const *>(data), size) {}

  InputView(byte const *const data, int const size)
      : data_(data), size_(size) {}

  byte const *data() const noexcept { return data_; }

  int size() const noexcept { return size_; }
};
}  // namespace dfc

#endif
