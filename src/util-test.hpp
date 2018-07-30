#ifndef DFC_TEST_UTILS_HPP
#define DFC_TEST_UTILS_HPP
#include <cstdint>

#include "immutable-pattern.hpp"

namespace dfc {
namespace test {
RawPattern twoBytePattern();
RawPattern fiveBytePattern();

RawPattern createPattern(char const* const str);
RawPattern createCaseInsensitivePattern(char const* const str);

ImmutablePattern createImmutablePattern(Pid const pid, char const* const str);
ImmutablePattern createCaseInsensitiveImmutablePattern(Pid const pid,
                                                       char const* const str);
}  // namespace test
}  // namespace dfc

#endif
