#include <catch2/catch_test_macros.hpp>

#include "gonia/shamos_hoey.hpp"

TEST_CASE("IsSimple", "[component]") {
  SECTION("trivial") {
    CHECK(is_simple(Polygon<int>{{{0, 0}}}));
    CHECK(is_simple(Polygon<int>{{{0, 0}, {1, 0}}}));
  }

  SECTION("figure3") {
    // clang-format off
    CHECK(intersect(std::vector<Segment<int>>{{
        {{ 0, 17}, {22,  9}}, // A
        {{ 3, 12}, {15, 11}}, // B
        {{ 7,  4}, {26, 14}}, // C
        {{ 8, 10}, {12,  9}}, // D
        {{11, 16}, {16, 15}}, // E
    }}));
    // clang-format on
  }
}
