// Copyright (C) 2023 Severin Strobl <git@severin-strobl.de>
//
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

#include "gonia/shamos_hoey.hpp"

TEST_CASE("Intersect", "[component]") {
  SECTION("figure3") {
    CHECK(intersect(std::vector<Segment<int>>{{
        {{0, 17}, {22, 9}},    // A
        {{3, 12}, {15, 11}},   // B
        {{7, 4}, {26, 14}},    // C
        {{8, 10}, {12, 9}},    // D
        {{11, 16}, {16, 15}},  // E
    }}));
  }
}
