#include "catch.hpp"

#include "direct-filter-collection.hpp"
#include "direct-filter.hpp"
#include "util-test.hpp"

using dfc::DirectFilter;
using dfc::DirectFilterCollection;
using dfc::test::createPattern;

TEST_CASE(
    "DF collection returns the maximum byte count required for indexing") {
  DirectFilterCollection<DirectFilter<uint8_t>, DirectFilter<uint32_t>,
                         DirectFilter<uint16_t>>
      collection;

  REQUIRE(collection.indexByteCount() == 4);
}

TEST_CASE("DF collection iterates over all filters when adding patterns") {
  DirectFilterCollection<DirectFilter<uint8_t>, DirectFilter<uint16_t>>
      collection;

  collection.addPattern(createPattern("12"));

  REQUIRE(collection.contains("1") == false);
  REQUIRE(collection.contains("12") == true);
}