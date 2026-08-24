#pragma once

#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <glm/glm.hpp>
#include "lib/util/vertex.h"

static constexpr float SLICE_EPSILON = 1e-6f;

typedef struct
{
  glm::vec3 origin;
  glm::vec3 normal;
} Plane;

struct IndexedPoint
{
  ushort index;
  glm::vec3 position;
  float signed_distance;
};

struct ProjectedPoint
{
  float x;
  float y;
  ushort index;
};

static void appendUniquePoint(std::vector<IndexedPoint> &points, const IndexedPoint &candidate)
{
  for (const IndexedPoint &point : points)
  {
    glm::vec3 delta = point.position - candidate.position;
    if (glm::dot(delta, delta) <= (SLICE_EPSILON * SLICE_EPSILON * 16.0f))
      return;
  }
  points.push_back(candidate);
}

static float orient2D(const ProjectedPoint &a, const ProjectedPoint &b, const ProjectedPoint &c)
{
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static std::vector<ushort> convexHull2D(std::vector<ProjectedPoint> points)
{
  if (points.size() < 3)
    return {};

  std::sort(points.begin(), points.end(), [](const ProjectedPoint &a, const ProjectedPoint &b) {
    if (a.x == b.x)
      return a.y < b.y;
    return a.x < b.x;
  });

  std::vector<ProjectedPoint> unique_points;
  unique_points.reserve(points.size());
  for (const ProjectedPoint &point : points)
  {
    if (!unique_points.empty())
    {
      const ProjectedPoint &last = unique_points.back();
      if (glm::abs(point.x - last.x) <= SLICE_EPSILON && glm::abs(point.y - last.y) <= SLICE_EPSILON)
        continue;
    }
    unique_points.push_back(point);
  }

  if (unique_points.size() < 3)
    return {};

  std::vector<ProjectedPoint> lower;
  for (const ProjectedPoint &point : unique_points)
  {
    while (lower.size() >= 2)
    {
      const size_t n = lower.size();
      if (orient2D(lower[n - 2], lower[n - 1], point) > SLICE_EPSILON)
        break;
      lower.pop_back();
    }
    lower.push_back(point);
  }

  std::vector<ProjectedPoint> upper;
  for (size_t i = unique_points.size(); i > 0; --i)
  {
    const ProjectedPoint &point = unique_points[i - 1];
    while (upper.size() >= 2)
    {
      const size_t n = upper.size();
      if (orient2D(upper[n - 2], upper[n - 1], point) > SLICE_EPSILON)
        break;
      upper.pop_back();
    }
    upper.push_back(point);
  }

  lower.pop_back();
  upper.pop_back();
  lower.insert(lower.end(), upper.begin(), upper.end());

  std::vector<ushort> hull_indices;
  hull_indices.reserve(lower.size());
  for (const ProjectedPoint &point : lower)
    hull_indices.push_back(point.index);
  return hull_indices;
}

static std::vector<ushort> convexWrapCutPoints(const std::vector<IndexedPoint> &cut_points, const Plane &plane)
{
  if (cut_points.size() < 3)
    return {};

  glm::vec3 axis = glm::abs(plane.normal.x) < 0.9f ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 u = glm::normalize(glm::cross(axis, plane.normal));
  glm::vec3 v = glm::cross(plane.normal, u);

  std::vector<ProjectedPoint> projected;
  projected.reserve(cut_points.size());
  for (const IndexedPoint &point : cut_points)
  {
    projected.push_back(ProjectedPoint{
        .x = glm::dot(point.position, u),
        .y = glm::dot(point.position, v),
        .index = point.index,
    });
  }

  return convexHull2D(projected);
}

static float signedDistanceToPlane(const glm::vec3 &point, const Plane &plane)
{
  return glm::dot(plane.normal, point - plane.origin);
}

static bool isInsideForSide(float signed_distance, bool keep_positive)
{
  return keep_positive ? signed_distance >= -SLICE_EPSILON : signed_distance <= SLICE_EPSILON;
}

static IndexedPoint makeIntersectionPoint(
  const IndexedPoint &from, 
  const IndexedPoint &to, 
  const Plane &plane, 
  std::vector<glm::vec3> *all_vertices,
  std::vector<IndexedPoint> &cut_points)
{
  float denom = from.signed_distance - to.signed_distance;
  float t = 0.0f;
  if (glm::abs(denom) > SLICE_EPSILON)
    t = from.signed_distance / denom;
  t = glm::clamp(t, 0.0f, 1.0f);

  glm::vec3 intersection = from.position + (to.position - from.position) * t;
  ushort intersection_index = static_cast<ushort>(all_vertices->size());
  all_vertices->push_back(intersection);

  IndexedPoint hit{
      .index = intersection_index,
      .position = intersection,
      .signed_distance = signedDistanceToPlane(intersection, plane),
  };
  appendUniquePoint(cut_points, hit);
  return hit;
}

static std::vector<IndexedPoint> clipTriangleToSide(
  const std::array<IndexedPoint, 3> &triangle, 
  const Plane &plane,
  bool keep_positive, 
  std::vector<glm::vec3> *all_vertices,
  std::vector<IndexedPoint> &cut_points)
{
  std::vector<IndexedPoint> clipped;
  clipped.reserve(4);

  for (size_t i = 0; i < triangle.size(); ++i)
  {
    const IndexedPoint &current = triangle[i];
    const IndexedPoint &next = triangle[(i + 1) % triangle.size()];
    bool current_inside = isInsideForSide(current.signed_distance, keep_positive);
    bool next_inside = isInsideForSide(next.signed_distance, keep_positive);

    if (current_inside && next_inside)
    {
      clipped.push_back(next);
      continue;
    }

    if (current_inside && !next_inside)
    {
      clipped.push_back(makeIntersectionPoint(current, next, plane, all_vertices, cut_points));
      continue;
    }

    if (!current_inside && next_inside)
    {
      clipped.push_back(makeIntersectionPoint(current, next, plane, all_vertices, cut_points));
      clipped.push_back(next);
      continue;
    }
  }

  return clipped;
}

static void appendTriangulatedPolygon(const std::vector<IndexedPoint> &polygon, std::vector<ushort> &indices)
{
  if (polygon.size() < 3)
    return;

  for (size_t i = 1; i + 1 < polygon.size(); ++i)
  {
    indices.push_back(polygon[0].index);
    indices.push_back(polygon[i].index);
    indices.push_back(polygon[i + 1].index);
  }
}