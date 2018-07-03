#include "catch.hpp"

#include "df-initializer.hpp"
#include "indexer.hpp"
#include "segmenter.hpp"
#include "util-test.hpp"

using dfc::test::twoBytePattern;

namespace {
TEST_CASE("Empty without patterns") {
  dfc::DirectFilterInitializer<uint16_t> init(1, 3);

  auto df = init.df();

  int ors = 0;
  for (auto const byte : df.filter()) {
    ors |= byte;
  }

  REQUIRE(ors == 0);
}

TEST_CASE("Sets bit if pattern is within the size constraint") {
  dfc::DirectFilterInitializer<uint16_t> init(1, 3);

  init.addPattern(twoBytePattern());

  auto const df = init.df();

  int ors = 0;
  for (auto const byte : df.filter()) {
    ors |= byte;
  }

  REQUIRE(ors != 0);
}

TEST_CASE("Does not set bit if pattern is outside the size constraint") {
  dfc::DirectFilterInitializer<uint16_t> init(3, 5);

  init.addPattern(twoBytePattern());

  auto const df = init.df();

  int ors = 0;
  for (auto const byte : df.filter()) {
    ors |= byte;
  }

  REQUIRE(ors == 0);
}
}  // namespace
