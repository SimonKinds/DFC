#include "byte.hpp"

namespace dfc {
namespace indexer {
template <typename T>
class Indexer {
 public:
  // convenience function
  inline auto index(const char* in) const noexcept {
    return index(reinterpret_cast<const byte*>(in));
  }

  inline auto index(const byte* in) const noexcept {
    auto underlying = static_cast<const T&>(*this);
    return underlying.index(in);
  }
};

namespace df {
class TwoByte : public Indexer<TwoByte> {
 public:
  using Size = uint16_t;

  inline Size index(const byte* in) const noexcept {
    return *reinterpret_cast<const Size*>(in);
  }
};

class FourByteHash : public Indexer<FourByteHash> {
 public:
  using Size = uint16_t;

  inline Size index(const byte* in) const noexcept {
    return *reinterpret_cast<const Size*>(in) * 13441;
  }
};

}  // namespace df
}  // namespace indexer
}  // namespace dfc
