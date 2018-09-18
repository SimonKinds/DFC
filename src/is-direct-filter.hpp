#ifndef DFC_IS_DIRECT_FILTER_HPP
#define DFC_IS_DIRECT_FILTER_HPP

#include <type_traits>

#include "direct-filter-interface.hpp"

namespace dfc {

template <typename DF>
using is_direct_filter = std::is_base_of<DirectFilterInterface, DF>;

}  // namespace dfc
#endif
