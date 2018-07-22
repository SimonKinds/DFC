#include "catch.hpp"

#include "indexer.hpp"

using dfc::CompactTableIndexer;
using dfc::DirectFilterIndexer;

namespace {
TEST_CASE("DF indexer shifts by 3 bits") {
  DirectFilterIndexer<int, 1, int> indexer;
  REQUIRE(indexer.index(0xff) == 0x1f);
}
TEST_CASE("DF indexer hash before shift") {
  auto const hash = 1u << 3u;
  DirectFilterIndexer<int, hash, int> indexer;
  REQUIRE(indexer.index(0xff) == 0xff);
}

TEST_CASE("CT indexer uses the mask") {
  auto const hash = 1;
  auto const mask = 0xfe;
  CompactTableIndexer<int, hash, mask> indexer;

  REQUIRE(indexer.index(0xff) == mask);
}
TEST_CASE("CT indexer hashes before mask") {
  auto const hash = 2;
  auto const mask = 0xff;
  auto const expected = 0xfe;

  CompactTableIndexer<int, hash, mask> indexer;

  REQUIRE(indexer.index(0xff) == expected);
}
}  // namespace
