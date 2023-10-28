#pragma once

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <set>
#include <stdexcept>
#include <vector>

template <typename T> struct Point {
  friend constexpr auto operator<=>(const Point &lhs,
                                    const Point &rhs) = default;

  T x;
  T y;
};

template <typename T> struct Segment {
  friend constexpr auto operator<=>(const Segment &lhs,
                                    const Segment &rhs) = default;

  Point<T> first;
  Point<T> second;
};

template <typename T> struct Polygon {
  std::vector<Point<T>> vertices;
};

template <typename T> struct Endpoint {
  friend constexpr auto operator<=>(const Endpoint &lhs,
                                    const Endpoint &rhs) = default;

  Point<T> vertex;
  std::size_t segment_idx = std::size_t{-1UL};
  bool start = false;
};

template <typename T>
constexpr T signed_area(const Point<T> &a, const Point<T> &b,
                        const Point<T> &c) {
  return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}

template <typename T>
constexpr bool intersect(const Segment<T> &lhs, const Segment<T> &rhs) {
  // identical segments
  if (lhs == rhs || lhs == Segment<T>(rhs.second, rhs.first)) {
    return true;
  }

  const auto a1 = signed_area(lhs.first, lhs.second, rhs.second);
  const auto a2 = signed_area(lhs.first, lhs.second, rhs.first);
  if (a1 * a2 >= 0) {
    return false;
  }

  const auto a3 = signed_area(rhs.first, rhs.second, lhs.first);
  const auto a4 = a3 + a2 - a1;

  return a3 * a4 < 0;
}

template<typename T>
inline bool intersect(const std::vector<Segment<T>> &segments) {
  using Endpoint = Endpoint<T>;
  std::vector<Endpoint> endpoints{};
  endpoints.reserve(segments.size() * 2);
  for (auto idx = 0U; const auto &segment : segments) {
    const auto flipped = segment.first > segment.second;
    endpoints.emplace_back(segment.first, idx, !flipped);
    endpoints.emplace_back(segment.second, idx, flipped);

    ++idx;
  }

  std::ranges::sort(endpoints);

  using Segment = Segment<T>;
  std::set<Segment, std::less<Segment>> sweep_line;
  for (const auto &endpoint : endpoints) {
    const auto segment = segments[endpoint.segment_idx];

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

template<typename T>
inline bool is_simple(const Polygon<T> &polygon) {
  if (polygon.vertices.size() < 3U) {
    return true;
  }

  if (polygon.vertices.front() != polygon.vertices.back()) {
    throw std::invalid_argument{"polygon must be closed"};
  }

  using Segment = Segment<T>;
  std::vector<Segment> segments{};
  segments.reserve(polygon.vertices.size() - 1);

  std::ranges::adjacent_find(polygon.vertices,
                             [&](const auto &first, const auto &second) {
                               segments.emplace_back(first, second);

                               return false;
                             });

  return !intersect(segments);
}
