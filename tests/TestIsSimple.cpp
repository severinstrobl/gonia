#include <catch2/catch_test_macros.hpp>

#include "gonia/shamos_hoey.hpp"

TEST_CASE("IsSimple", "{component}") {
  SECTION("trivial") {
    CHECK(is_simple(Polygon<int>{{{0, 0}}}));
    CHECK(is_simple(Polygon<int>{{{0, 0}, {1, 0}}}));
  }

  SECTION("duplicate") {
    CHECK_FALSE(is_simple(Polygon<int>{{{0, 0}, {1, 0}, {0, 0}}}));
  }

  SECTION("triangle") {
    CHECK(is_simple(Polygon<int>{{{0, 0}, {2, 0}, {1, 1}, {0, 0}}}));
  }

  SECTION("quadrilateral") {
    CHECK(is_simple(Polygon<int>{{{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}}}));
  }

  SECTION("crossed") {
    CHECK(!is_simple(Polygon<int>{{{0, 0}, {1, 1}, {1, 0}, {0, 1}, {0, 0}}}));
  }

  SECTION("duplicates") {
    CHECK(is_simple(Polygon<int>{{{0, 0}, {1, 0}, {1, 1}, {1, 1}, {0, 1}, {0, 0}}}));
  }

  SECTION("geopoly") {
    // clang-format off
    const auto poly = Polygon<double>{{
      {90.39974817514972, 23.873991637113175},
      {90.40572498813789, 23.872911671285607},
      {90.40575223587952, 23.874804784092955},
      {90.4053503301788, 23.875873772177464},
      {90.40030947983682, 23.87861920694433},
      {90.39621276706808, 23.878127145947108},
      {90.3921160542993, 23.87747811375598},
      {90.39023118436477, 23.882439106594358},
      {90.38922437638114, 23.88382080428333},
      {90.38693010807042, 23.88390756033242},
      {90.38277413010324, 23.877399629023042},
      {90.37991123348131, 23.875909945040224},
      {90.37897952735014, 23.873517645288647},
      {90.37864863603832, 23.871792452376454},
      {90.37969103574207, 23.869125362621826},
      {90.3834212654788, 23.86583190806136},
      {90.38663651108469, 23.86556034446246},
      {90.39203703403477, 23.865488168279917},
      {90.3940094372637, 23.865251113185526},
      {90.3959818404927, 23.86548501039226},
      {90.39992664695066, 23.865638836372778},
      {90.40017732859211, 23.874030880518813},
      // {90.39974817514972, 23.873991637113175},
      // {90.39974817514972, 23.873991637113175},
      {90.39974817514972, 23.873991637113175}
    }};
    // clang-format on

    CHECK(!is_simple(poly));
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
      {53.936102, 19.63358},   {84.819935, 22.975206}
    }};
    // clang-format on

    CHECK(is_simple(poly));
  }
}
