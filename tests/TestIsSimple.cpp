// Copyright (C) 2023 Severin Strobl <git@severin-strobl.de>
//
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <ranges>
#include <span>

#include "gonia/shamos_hoey.hpp"

using Catch::Matchers::ContainsSubstring;

TEST_CASE("IsSimple", "[component]") {
  SECTION("degenerate") {
    CHECK_THROWS_WITH(is_simple(Polygon<int>{{{0, 0}}}),
                      ContainsSubstring("degenerate polygon detected"));

    CHECK_THROWS_WITH(is_simple(Polygon<int>{{{0, 0}, {1, 0}}}),
                      ContainsSubstring("degenerate polygon detected"));
  }

  SECTION("tri") { CHECK(is_simple(Polygon<int>{{{0, 0}, {1, 0}, {1, 1}}})); }

  SECTION("tri_degenerate") {
    CHECK_FALSE(is_simple(Polygon<int>{{{0, 0}, {1, 0}, {1, 0}}}));
  }

  SECTION("quad") {
    CHECK(is_simple(Polygon<int>{{{0, 0}, {1, 0}, {1, 1}, {0, 1}}}));
  }

  SECTION("quad_degenerate") {
    CHECK_FALSE(is_simple(Polygon<int>{{{0, 0}, {1, 0}, {1, 0}, {0, 1}}}));
  }

  SECTION("crossed") {
    CHECK(!is_simple(Polygon<int>{{{0, 0}, {1, 1}, {1, 0}, {0, 1}}}));
  }

  SECTION("duplicates") {
    CHECK(!is_simple(Polygon<int>{{{0, 0}, {1, 0}, {1, 1}, {1, 1}, {0, 1}}}));
  }

  ///  |\     /|
  ///  | \   / |
  ///  |__\./__|
  SECTION("touching_vertex") {
    CHECK(!is_simple(
        Polygon<int>{{{-1, 0}, {0, 0}, {1, 0}, {1, 1}, {0, 0}, {-1, 1}}}));
  }

  ///  |\    /|
  ///  | \  / |
  ///  |__\/__|
  SECTION("touching_edge") {
    CHECK(!is_simple(Polygon<int>{{{-1, 0}, {1, 0}, {1, 1}, {0, 0}, {-1, 1}}}));
  }

  SECTION("jagged") {
    // clang-format off
    const auto poly = Polygon<double>{{
      {84.819935, 22.975206},  {84.819935, 29.658459},
      {76.584247, 26.985158},  {78.643169, 35.005061},
      {103.350235, 28.990134}, {84.819935, 20.301905},
      {45.700414, 16.291953},  {25.111192, 19.63358},
      {27.170114, 22.975206},  {15.359907, 25.509273},
      {6.580892, 18.965254},   {74.525324, 12.950327},
      {119.821613, 32.33176},  {82.761013, 37.678362},
      {66.289636, 47.034916},  {39.523647, 39.683338},
      {49.818258, 35.005061},  {37.464725, 35.005061},
      {29.514997, 44.036735},  {10.984697, 38.690133},
      {53.936102, 28.990134},  {58.053947, 43.024964},
      {76.584247, 37.678362},  {64.230713, 35.673387},
      {64.230713, 24.980182},  {47.759336, 24.980182},
      {37.464725, 30.326784},  {0.404125, 30.326784},
      {53.936102, 19.63358}
    }};
    // clang-format on

    CHECK(is_simple(poly));
  }
}
