#include "catch.hpp"

#include "indexer.hpp"

using namespace dfc::indexer;

TEST_CASE("2B indexer uses the first 2 bytes") {
  Indexer<df::TwoByte> indexer;

  uint16_t expected = 'e' << 8 | 't';

  REQUIRE(indexer.index("text") == expected);
}

TEST_CASE("4B hash outputs 2B") {
  Indexer<df::FourByteHash> indexer;

  REQUIRE(sizeof(indexer.index("text")) == 2);
}
