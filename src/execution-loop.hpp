#ifndef DFC_EXECUTION_LOOP
#define DFC_EXECUTION_LOOP

#include <tuple>

#include "immutable-pattern.hpp"
#include "input-view.hpp"
#include "on-matcher.hpp"

namespace dfc {
/**
 * The main execution loop of the library.
 * It will contain all the direct filters and compact tables,
 * and will iterate over a given input and match at each location
 *
 * Every template parameter should be a MatchPath.
 */

template <typename... MatchPaths>
class ExecutionLoop {
  using TupleType = std::tuple<MatchPaths...>;
  TupleType matchPaths_{};

 public:
  /**
   * Add the given pattern to all match paths
   */
  void addPattern(ImmutablePattern const &pattern) {
    addPattern(pattern,
               std::make_index_sequence<std::tuple_size<TupleType>::value>());
  }

 private:
  template <std::size_t... Is>
  void addPattern(ImmutablePattern const &pattern, std::index_sequence<Is...>) {
    (std::get<Is>(matchPaths_).addPattern(pattern), ...);
  }

 public:
  /**
   * Finds all the occurences of the added patterns in the given input
   */
  void match(InputView const &initialInput, OnMatcher const &onMatcher) const {
    InputView input = initialInput;
    while (input.size() > largestIndexByteCount()) {
      unsafeMatch(
          input, onMatcher,
          std::make_index_sequence<std::tuple_size<TupleType>::value>());

      input = input.next();
    }

    safeMatch(input, onMatcher);
  }

 private:
  template <std::size_t... Is>
  inline void unsafeMatch(InputView const &input, OnMatcher const &onMatcher,
                          std::index_sequence<Is...>) const {
    (std::get<Is>(matchPaths_).unsafeMatch(input, onMatcher), ...);
  }

  void safeMatch(InputView const &initialInput,
                 OnMatcher const &onMatcher) const {
    InputView input = initialInput;
    while (input.size() > 0) {
      safeMatch(input, onMatcher,
                std::make_index_sequence<std::tuple_size<TupleType>::value>());

      input = input.next();
    }
  }

  template <std::size_t... Is>
  inline void safeMatch(InputView const &input, OnMatcher const &onMatcher,
                        std::index_sequence<Is...>) const {
    (std::get<Is>(matchPaths_).match(input, onMatcher), ...);
  }

  constexpr int largestIndexByteCount() const noexcept {
    return largestIndexByteCount(
        std::make_index_sequence<std::tuple_size<TupleType>::value>());
  }

  template <std::size_t I, std::size_t... Is>
  constexpr int largestIndexByteCount(std::index_sequence<I, Is...>) const
      noexcept {
    if constexpr (sizeof...(Is) == 0) {
      return std::get<I>(matchPaths_).largestIndexByteCount();
    } else {
      return std::max(std::get<I>(matchPaths_).largestIndexByteCount(),
                      largestIndexByteCount(std::index_sequence<Is...>()));
    }
  }
};
}  // namespace dfc

#endif
