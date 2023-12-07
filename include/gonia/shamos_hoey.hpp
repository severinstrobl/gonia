// Copyright (C) 2023 Severin Strobl <git@severin-strobl.de>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <set>
#include <stdexcept>
#include <vector>

template<typename T>
struct Point {
  friend constexpr auto operator<=>(const Point& lhs,
                                    const Point& rhs) = default;

  T x;
  T y;
};

template<typename T>
constexpr auto dot(const Point<T>& lhs, const Point<T>& rhs) -> T {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<typename T>
constexpr auto operator-(const Point<T>& lhs, const Point<T>& rhs) -> Point<T> {
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}

template<typename T>
struct Segment {
  friend constexpr auto operator<=>(const Segment& lhs,
                                    const Segment& rhs) = default;

  Point<T> first;
  Point<T> second;
};

template<typename T>
struct Polygon {
  std::vector<Point<T>> vertices;
};

template<typename T>
struct Endpoint {
  friend constexpr auto operator<=>(const Endpoint& lhs,
                                    const Endpoint& rhs) = default;

  Point<T> vertex;
  std::size_t segment_idx = std::size_t{-1UL};
  bool start = false;
};

template<typename T>
constexpr auto signed_area(const Point<T>& a, const Point<T>& b,
                           const Point<T>& c) -> T {
  return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}

// Test if the open line segments `lhs` and `rhs` have a point in common. For an
// open line segment, the endpoints are not considered to be part of the line
// segment. Consequently, degenerate line segments formed by coinciding
// endpoints do not intersect.
template<typename T>
constexpr auto intersect(const Segment<T>& lhs, const Segment<T>& rhs) -> bool {
  // identical segments
  if (lhs == rhs || lhs == Segment<T>(rhs.second, rhs.first)) {
    return true;
  }

  const auto area1 = signed_area(lhs.first, lhs.second, rhs.second);
  const auto area2 = signed_area(lhs.first, lhs.second, rhs.first);

  // the endpoints of the RHS segment are truly on the same side of the LHS
  // segment
  if (area1 * area2 > T{0}) {
    return false;
  }

  const auto area3 = signed_area(rhs.first, rhs.second, lhs.first);
  const auto area4 = area3 + area2 - area1;

  // the endpoints of the LHS segment are truly on the same side of the RHS
  // segment
  if (area3 * area4 > T{0}) {
    return false;
  }

  // segments definitively intersect if the endpoints of one segment are truly
  // on opposite sides of the other segment
  if (area1 * area2 < T{0} || area3 * area4 < T{0}) {
    return true;
  }

  // for colinear segments all areas vanish and overlap between segments has to
  // be checked
  if (std::ranges::all_of(
          std::array{area1, area2, area3, area4}, [](const auto& area) {
            return std::abs(area) <= std::numeric_limits<T>::epsilon();
          })) {
    for (const auto& p : {rhs.first, rhs.second}) {
      if (dot(lhs.first - lhs.second, p - lhs.second) >= T{0} &&
          dot(lhs.second - lhs.first, p - lhs.first) >= T{0}) {
        return true;
      }
    }

    for (const auto& p : {lhs.first, lhs.second}) {
      if (dot(rhs.first - rhs.second, p - rhs.second) >= T{0} &&
          dot(rhs.second - rhs.first, p - rhs.first) >= T{0}) {
        return true;
      }
    }
  }

  return false;
}

namespace detail {

template<typename T>
inline auto intersect_impl(const std::vector<Segment<T>>& segments,
                           const std::vector<Endpoint<T>>& sorted_endpoints)
    -> bool {
  using Segment = Segment<T>;
  std::set<Segment, std::less<Segment>> sweep_line;
  for (const auto& endpoint : sorted_endpoints) {
    const auto& segment = segments[endpoint.segment_idx];
    if (endpoint.start) {
      const auto [pos, added] =
          sweep_line.emplace(segment.first, segment.second);

      if (!added) {
        throw std::logic_error{"encountered duplicate segment in sweep line"};
      }

      if (pos != std::prev(sweep_line.end()) &&
          intersect(segment, *std::next(pos))) {
        return true;
      }

      if (pos != sweep_line.begin() && intersect(segment, *std::prev(pos))) {
        return true;
      }
    } else {
      const auto pos = sweep_line.find({segment.first, segment.second});
      if (pos == sweep_line.end()) {
        throw std::logic_error{"failed to find expected segment in sweep line"};
      }

      if (std::next(pos) != sweep_line.end() && pos != sweep_line.begin() &&
          intersect(*std::next(pos), *std::prev(pos))) {
        return true;
      }

      sweep_line.erase({segment.first, segment.second});
    }
  }

  return false;
}

}  // namespace detail

template<typename T>
inline auto intersect(const std::vector<Segment<T>>& segments) -> bool {
  if (segments.size() < 2u) {
    return false;
  }

  std::vector<Endpoint<T>> endpoints{};
  endpoints.reserve(segments.size() * 2);
  for (auto idx = 0u; const auto& segment : segments) {
    const auto flipped = segment.first > segment.second;
    endpoints.emplace_back(segment.first, idx, !flipped);
    endpoints.emplace_back(segment.second, idx, flipped);

    ++idx;
  }

  std::ranges::sort(endpoints);

  return detail::intersect_impl(segments, endpoints);
}

template<typename T>
inline auto is_simple(const Polygon<T>& polygon) -> bool {
  if (polygon.vertices.size() < 3u) {
    throw std::invalid_argument{
        "degenerate polygon detected, at least three points required"};
  }

  if (polygon.vertices.front() == polygon.vertices.back()) {
    throw std::invalid_argument{"polygon must not be closed"};
  }

  using Segment = Segment<T>;
  std::vector<Segment> segments{};
  segments.reserve(polygon.vertices.size());

  const auto duplicates = std::ranges::adjacent_find(
      polygon.vertices, [&](const auto& first, const auto& second) {
        if (first == second) {
          return true;
        }

        segments.emplace_back(first, second);
        return false;
      });

  if (duplicates != polygon.vertices.end()) {
    return false;
  }

  // final segment connecting first and last point
  segments.emplace_back(polygon.vertices.back(), polygon.vertices.front());

  std::vector<Endpoint<T>> endpoints{};
  endpoints.reserve(segments.size() * 2);
  for (auto idx = 0u; const auto& segment : segments) {
    const auto flipped = segment.first > segment.second;
    endpoints.emplace_back(segment.first, idx, !flipped);
    endpoints.emplace_back(segment.second, idx, flipped);

    ++idx;
  }

  std::ranges::sort(endpoints);

  // each point coordinate must appear exactly twice in the sorted endpoints
  for (auto it = endpoints.begin(); it != endpoints.end();) {
    const auto range_end = std::find_if_not(
        it, endpoints.end(),
        [&](const auto& ep) { return it->vertex == ep.vertex; });

    if (std::distance(it, range_end) != 2u) {
      return false;
    }

    it = range_end;
  }

  // TODO(severin): ensure polygon has no holes

  return !detail::intersect_impl(segments, endpoints);
}
