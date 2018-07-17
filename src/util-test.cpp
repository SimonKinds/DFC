#include "util-test.hpp"

namespace dfc {
namespace test {
RawPattern createPattern(char const* const str) {
  return RawPattern(reinterpret_cast<byte const*>(str), std::strlen(str));
}
RawPattern twoBytePattern() { return createPattern("te"); }
RawPattern fiveBytePattern() { return createPattern("testx"); }
RawPattern createCaseInsensitivePattern(char const* const str) {
  return RawPattern(reinterpret_cast<byte const*>(str), std::strlen(str),
                    false);
}
}  // namespace test
}  // namespace dfc
