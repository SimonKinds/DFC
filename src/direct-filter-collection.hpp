#ifndef DFC_DF_COLLECTION_HPP
#define DFC_DF_COLLECTION_HPP

#include <algorithm>
#include <tuple>

#include "direct-filter.hpp"
#include "is-direct-filter.hpp"

namespace dfc {

template <typename... Ts>
class DirectFilterCollection final : public DirectFilterInterface {
  static_assert((is_direct_filter<Ts>::value && ...),
                "Must be a direct filter");

  using TupleType = std::tuple<Ts...>;
  TupleType dfs_{};

 public:
  inline int indexByteCount() const noexcept final {
    return getMaxByteCount(std::make_index_sequence<dfCount()>());
  }

  inline bool contains(char const *const in) const noexcept {
    return contains(reinterpret_cast<byte const *>(in));
  }

  inline bool contains(byte const *const in) const noexcept final {
    return contains(in, std::make_index_sequence<dfCount()>());
  }

  void addPattern(Pattern const &pattern) final {
    addPattern(pattern, std::make_index_sequence<dfCount()>());
  }

 private:
  static constexpr int dfCount() { return std::tuple_size<TupleType>::value; }

  template <std::size_t I, std::size_t... Is>
  constexpr int getMaxByteCount(std::index_sequence<I, Is...>) const noexcept {
    if constexpr (sizeof...(Is) == 0) {
      return std::get<I>(dfs_).indexByteCount();
    } else {
      return std::max(std::get<I>(dfs_).indexByteCount(),
                      getMaxByteCount(std::index_sequence<Is...>()));
    }
  }

  template <std::size_t... Is>
  constexpr bool contains(byte const *const in,
                          std::index_sequence<Is...>) const noexcept {
    return (std::get<Is>(dfs_).contains(in) && ...);
  }

  template <std::size_t... Is>
  void addPattern(Pattern const &pattern, std::index_sequence<Is...>) {
    (std::get<Is>(dfs_).addPattern(pattern), ...);
  }
};
}  // namespace dfc

#endif
