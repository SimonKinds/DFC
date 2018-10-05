#ifndef DFC_MATCH_DATA_FILE_HPP
#define DFC_MATCH_DATA_FILE_HPP

#include <cstdio>
#include <functional>
#include <memory>

#include "byte.hpp"

namespace dfc {
namespace match {
class DataFile {
  std::string path_;
  std::unique_ptr<byte[]> data_;
  int64_t size_;

 public:
  explicit DataFile(std::string path) : path_(std::move(path)) {}
  DataFile(std::string path, std::unique_ptr<byte[]> data, int64_t const size)
      : path_(std::move(path)), data_(std::move(data)), size_(size) {}

  DataFile read() const noexcept {
    std::unique_ptr<std::FILE, std::function<void(std::FILE*)>> file(
        std::fopen(path_.c_str(), "rb"), [](FILE* file) { std::fclose(file); });

    if (file == nullptr) {
      return DataFile(path_);
    }

    std::fseek(file.get(), 0, SEEK_END);
    size_t size = ftell(file.get());
    std::fseek(file.get(), 0, SEEK_SET);

    auto data = std::make_unique<byte[]>(size);

    size_t readCount = std::fread(data.get(), size, 1, file.get());

    if (readCount != 1) {
      std::perror("Could not read file");
      return DataFile(path_);
    }

    return DataFile(path_, std::move(data), static_cast<int64_t>(size));
  }

  byte const* data() const noexcept { return data_.get(); }
  int64_t size() const noexcept { return size_; }
};
}  // namespace match
}  // namespace dfc

#endif
