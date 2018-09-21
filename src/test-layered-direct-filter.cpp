#include "catch.hpp"

#include "flat-direct-filter.hpp"
#include "layered-direct-filter.hpp"
#include "util-test.hpp"

using dfc::FlatDirectFilter;
using dfc::LayeredDirectFilter;
using dfc::test::createPattern;

TEST_CASE(
    "DF collection returns the maximum byte count required for indexing") {
  LayeredDirectFilter<FlatDirectFilter<uint8_t>, FlatDirectFilter<uint32_t>,
                      FlatDirectFilter<uint16_t>>
      collection;

  REQUIRE(collection.indexByteCount() == 4);
}

TEST_CASE("DF collection iterates over all filters when adding patterns") {
  LayeredDirectFilter<FlatDirectFilter<uint8_t>, FlatDirectFilter<uint16_t>>
      collection;

  collection.addPattern(createPattern("12"));

  REQUIRE(collection.contains("1") == false);
  REQUIRE(collection.contains("12") == true);
}
