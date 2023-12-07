// Copyright (C) 2023 Severin Strobl <git@severin-strobl.de>
//
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

#include "gonia/shamos_hoey.hpp"

TEST_CASE("BinaryIntersection", "[unit]") {
  SECTION("crossed") {
    ///      |
    ///   ___|___
    ///      |
    CHECK(intersect(Segment<float>{{-1, 0}, {1, 0}},
                    Segment<float>{{0, -1}, {0, 1}}));

    CHECK(intersect(Segment<float>{{0, -1}, {0, 1}},
                    Segment<float>{{-1, 0}, {1, 0}}));

    ///       /
    ///   ___/___
    ///     /
    CHECK(intersect(Segment<float>{{-1, 0}, {1, 0}},
                    Segment<float>{{-0.5, -1}, {0.5, 1}}));

    CHECK(intersect(Segment<float>{{-0.5, -1}, {0.5, 1}},
                    Segment<float>{{-1, 0}, {1, 0}}));
  }

  SECTION("perpendicular") {
    ///      |
    ///      |
    ///   ___|___
    CHECK(intersect(Segment<float>{{-1, 0}, {1, 0}},
                    Segment<float>{{0, 0}, {0, 1}}));

    CHECK(intersect(Segment<float>{{0, 0}, {0, 1}},
                    Segment<float>{{-1, 0}, {1, 0}}));

    ///        /
    ///       /
    ///   ___/___
    CHECK(intersect(Segment<float>{{-1, 0}, {1, 0}},
                    Segment<float>{{0, 0}, {0.5, 1}}));

    CHECK(intersect(Segment<float>{{0, 0}, {0.5, 1}},
                    Segment<float>{{-1, 0}, {1, 0}}));

    ///        /
    ///       /
    ///      /
    ///   _______
    CHECK_FALSE(intersect(Segment<float>{{-1, 0}, {1, 0}},
                          Segment<float>{{0, 0.5}, {0.5, 1.5}}));

    CHECK_FALSE(intersect(Segment<float>{{0, 0.5}, {0.5, 1.5}},
                          Segment<float>{{-1, 0}, {1, 0}}));
  }

  SECTION("corner") {
    ///   |
    ///   |
    ///   |______
    CHECK_FALSE(intersect(Segment<float>{{0, 0}, {1, 0}},
                          Segment<float>{{0, 0}, {0, 1}}));

    CHECK_FALSE(intersect(Segment<float>{{0, 0}, {0, 1}},
                          Segment<float>{{0, 0}, {1, 0}}));

    ///     /
    ///    /
    ///   /______
    CHECK_FALSE(intersect(Segment<float>{{-1, 0}, {1, 0}},
                          Segment<float>{{-1, 0}, {0, 1}}));

    CHECK_FALSE(intersect(Segment<float>{{-1, 0}, {0, 1}},
                          Segment<float>{{-1, 0}, {1, 0}}));

    ///     /
    ///    /
    ///   /  ______
    CHECK_FALSE(intersect(Segment<float>{{-1, 0}, {1, 0}},
                          Segment<float>{{-2, 0}, {0, 1}}));

    CHECK_FALSE(intersect(Segment<float>{{-2, 0}, {0, 1}},
                          Segment<float>{{-1, 0}, {1, 0}}));
  }

  ///   ____.__.____
  SECTION("overlapping") {
    CHECK(intersect(Segment<float>{{0, 0}, {2, 0}},
                    Segment<float>{{1, 0}, {3, 0}}));

    CHECK(intersect(Segment<float>{{1, 0}, {3, 0}},
                    Segment<float>{{0, 0}, {2, 0}}));
  }

  ///   ______   ______
  SECTION("colinear") {
    CHECK_FALSE(intersect(Segment<float>{{0, 0}, {1, 0}},
                          Segment<float>{{2, 0}, {3, 0}}));

    CHECK_FALSE(intersect(Segment<float>{{2, 0}, {3, 0}},
                          Segment<float>{{0, 0}, {1, 0}}));
  }

  /// =========
  SECTION("parallel") {
    CHECK_FALSE(intersect(Segment<float>{{-1, -0.5}, {1, -0.5}},
                          Segment<float>{{-1, 0.5}, {1, 0.5}}));
  }
}

TEST_CASE("Intersection", "[unit]") {
  // example corresponding to Fig. 3 of Shamos and Hoey, 1976
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
