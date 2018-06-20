#include "catch.hpp"

#include "indexer.hpp"

TEST_CASE("Direct Filter") {
  SECTION("2B indexer uses the first 2 bytes") {
    dfc::TwoByteDfIndexer indexer;

    uint16_t expected = ('e' << 8 | 't') >> 3;

    REQUIRE(indexer.index("text") == expected);
  }

  SECTION("4B hash outputs 2B") {
    dfc::FourByteHashDfIndexer indexer;

    REQUIRE(sizeof(indexer.index("text")) == 2);
  }
}
