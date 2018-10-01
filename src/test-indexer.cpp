#include "catch.hpp"

#include "indexer.hpp"

using dfc::CompactTableIndexer;
using dfc::DirectFilterIndexer;

namespace {
TEST_CASE("DF indexer shifts by 3 bits") {
  auto const hash = 1;

  DirectFilterIndexer<int, hash, int> indexer;
  REQUIRE(indexer.index(0xff) == 0x1f);
}
TEST_CASE("DF indexer hashes before shift") {
  auto const hash = 1u << 3u;

  DirectFilterIndexer<int, hash, int> indexer;
  // if the shift happens before the hash, we would lose the 3 LSB in the
  // process
  REQUIRE(indexer.index(0xff) == 0xff);
}

TEST_CASE(
    "DF indexer max return value is RetType shifted 3 bytes to the right") {
  DirectFilterIndexer<uint16_t, 1, uint8_t> indexer;
  REQUIRE(indexer.index(std::numeric_limits<uint16_t>::max()) ==
          (std::numeric_limits<uint8_t>::max() >> 3));
}

TEST_CASE("CT indexer uses the mask") {
  auto const hash = 1;
  auto const mask = 0xfe;
  CompactTableIndexer<int, hash, mask> indexer;

  // with a hash of 1, the output depends only on the mask and the input
  REQUIRE(indexer.index(0xff) == mask);
}
TEST_CASE("CT indexer hashes before mask") {
  auto const hash = 2;
  auto const mask = 0xff;

  CompactTableIndexer<int, hash, mask> indexer;

  // wish a hash of 2, the input will be shifted by 1 bit before using the mask
  REQUIRE(indexer.index(0xff) == 0xfe);
}
}  // namespace
