#include "catch.hpp"

#include "ct.hpp"
#include "df.hpp"
#include "match-path.hpp"
#include "util-test.hpp"

namespace {
using dfc::ImmutablePattern;
using dfc::SaveOnMatcher;
using dfc::test::createPattern;

using Df = dfc::DirectFilter<dfc::PatternRange<2, 100>, uint16_t, 1, uint16_t>;
using Ct = dfc::CompactTable<dfc::PatternRange<2, 100>, uint16_t, 1, 100>;

TEST_CASE("No match if no patterns are added") {
  SaveOnMatcher onMatcher;
  dfc::MatchPath<Df, Ct> path;

  std::string in("test");
  path.match(in.data(), in.size(), onMatcher);

  REQUIRE(onMatcher.matchedPids.empty());
}

TEST_CASE("Match if input equals pattern") {
  SaveOnMatcher onMatcher;
  std::string in("test");
  dfc::MatchPath<Df, Ct> path;

  dfc::Pid const pid = 123;
  path.addPattern(ImmutablePattern(pid, createPattern(in.data())));

  path.match(in.data(), in.size(), onMatcher);

  REQUIRE(onMatcher.matchedPids.size() == 1);
  REQUIRE(onMatcher.matchedPids[0] == pid);
}
}  // namespace
