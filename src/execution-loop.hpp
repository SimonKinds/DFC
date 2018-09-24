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
  void addPattern(ImmutablePattern const &pattern) {
    addPattern(pattern,
               std::make_index_sequence<std::tuple_size<TupleType>::value>());
  }

  inline void match(InputView const &initialInput,
                    OnMatcher const &onMatcher) const {
    InputView input = initialInput;
    while (input.size() > 0) {
      match(input, onMatcher,
            std::make_index_sequence<std::tuple_size<TupleType>::value>());

      input = input.next();
    }
  }

 private:
  template <std::size_t... Is>
  void addPattern(ImmutablePattern const &pattern, std::index_sequence<Is...>) {
    (std::get<Is>(matchPaths_).addPattern(pattern), ...);
  }

  template <std::size_t... Is>
  inline void match(InputView const &input, OnMatcher const &onMatcher,
                    std::index_sequence<Is...>) const {
    (std::get<Is>(matchPaths_).match(input, onMatcher), ...);
  }
};
}  // namespace dfc

#endif
