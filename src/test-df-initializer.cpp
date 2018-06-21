#include "catch.hpp"

#include "df-initializer.hpp"
#include "indexer.hpp"
#include "segmenter.hpp"

dfc::RawPattern twoBytePattern() {
  std::string string_pat("te");
  dfc::RawPattern pat(reinterpret_cast<const byte*>(string_pat.data()),
                      string_pat.size());

  return pat;
}

TEST_CASE("Empty without patterns") {
  dfc::DfInitializer<dfc::TwoByteDfIndexer> init(1, 3);

  auto df = init.df();

  int ors = 0;
  for (const auto byte : df.filter()) {
    ors |= byte;
  }

  REQUIRE(ors == 0);
}

TEST_CASE("Sets bit if pattern is within the size constraint") {
  dfc::DfInitializer<dfc::TwoByteDfIndexer> init(1, 3);

  init.addPattern(twoBytePattern());

  const auto df = init.df();

  int ors = 0;
  for (const auto byte : df.filter()) {
    ors |= byte;
  }

  REQUIRE(ors != 0);
}

TEST_CASE("Does not set bit if pattern is outside the size constraint") {
  dfc::DfInitializer<dfc::TwoByteDfIndexer> init(3, 5);

  init.addPattern(twoBytePattern());

  const auto df = init.df();

  int ors = 0;
  for (const auto byte : df.filter()) {
    ors |= byte;
  }

  REQUIRE(ors == 0);
}
