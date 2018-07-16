#include <string>
#include "catch.hpp"

#include "segmenter.hpp"

/**
 * All tests in this file assumes LITTLE ENDIAN
 */

namespace {
using dfc::Segmenter;
using OneByteSegmenter = Segmenter<uint8_t>;
using TwoByteSegmenter = Segmenter<uint16_t>;
using FourByteSegmenter = Segmenter<uint32_t>;

TEST_CASE("Segments out the desired amount of characters") {
  int const value = 0x12345678;
  byte const* in = reinterpret_cast<byte const*>(&value);

  REQUIRE(OneByteSegmenter().segment(in) == 0x78);
  REQUIRE(TwoByteSegmenter().segment(in) == 0x5678);
  REQUIRE(FourByteSegmenter().segment(in) == 0x12345678);
}

TEST_CASE("Permutations with upper and lower case characters") {
  auto const value = 'b' << 8 | 'a';
  byte const* in = reinterpret_cast<byte const*>(&value);
  std::array<uint16_t, 4> expectedPermutations = {
      'a' << 8 | 'b',
      'A' << 8 | 'b',
      'a' << 8 | 'B',
      'A' << 8 | 'B',
  };

  auto const permutations = TwoByteSegmenter().permutations(in);
  auto const countOccurrences = [=](auto val) {
    return std::count(std::cbegin(permutations), std::cend(permutations), val);
  };

  REQUIRE(expectedPermutations.size() == permutations.size());
  REQUIRE(countOccurrences(expectedPermutations[0]) == 1);
  REQUIRE(countOccurrences(expectedPermutations[1]) == 1);
  REQUIRE(countOccurrences(expectedPermutations[2]) == 1);
  REQUIRE(countOccurrences(expectedPermutations[3]) == 1);
}

}  // namespace
