#ifndef DFC_CHARACTER_TRANSFORMER_HPP
#define DFC_CHARACTER_TRANSFORMER_HPP

#include <array>
#include <limits>

namespace dfc {
class CharacterTransformer final {
  using CharToLowerMapCaseType =
      std::array<uint8_t, std::numeric_limits<uint8_t>::max() + 1>;

  static constexpr CharToLowerMapCaseType initializecharToLowerMapCase() {
    CharToLowerMapCaseType charToLowerMap{};
    for (int i = 0; i < static_cast<int>(charToLowerMap.size()); ++i) {
      int lower = i;
      if (i >= 65 && i <= 90) {
        lower = i + 32;
      }
      charToLowerMap[i] = lower;
    }

    return charToLowerMap;
  }

  CharToLowerMapCaseType const charToLowerMap = initializecharToLowerMapCase();

 public:
  inline uint8_t toLower(uint8_t val) const noexcept {
    return charToLowerMap[val];
  }
};
}  // namespace dfc

#endif
