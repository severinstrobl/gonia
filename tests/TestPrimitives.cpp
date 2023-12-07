// Copyright (C) 2023 Severin Strobl <git@severin-strobl.de>
//
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

#include "gonia/shamos_hoey.hpp"

TEST_CASE("Point", "[unit]") {
  SECTION("constructor") {
    STATIC_CHECK(Point<int>{0, 1}.x == 0);
    STATIC_CHECK(Point<int>{0, 1}.y == 1);
  }

  SECTION("equality") {
    STATIC_CHECK(Point<int>{0, 1} == Point<int>{0, 1});
    STATIC_CHECK(Point<int>{0, 1} != Point<int>{1, 0});
  }

  SECTION("ordering") {
    STATIC_CHECK(Point<int>{0, 1} < Point<int>{0, 2});
    STATIC_CHECK(Point<int>{0, 1} < Point<int>{1, 0});
    STATIC_CHECK(Point<int>{0, 1} > Point<int>{0, 0});
    STATIC_CHECK(Point<int>{1, 0} > Point<int>{0, 1});
  }
}
