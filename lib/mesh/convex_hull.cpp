#include "convex_hull.h"

#include <algorithm>
#include <cmath>
#include <unordered_set>

typedef struct
{
  ushort index;
  glm::vec3 pos;
} HullPoint;

typedef struct
{
  glm::vec3 normal;
  float d;
  std::vector<int> point_ids;
} HullPlane;

typedef struct
{
  float x;
  float y;
  int point_id;
} ProjectedPoint;

static float orient2D(const ProjectedPoint &a, const ProjectedPoint &b, const ProjectedPoint &c)
{
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static std::vector<int> convexHull2D(std::vector<ProjectedPoint> points)
{
  if (points.size() < 3)
    return {};

  std::sort(points.begin(), points.end(), [](const ProjectedPoint &a, const ProjectedPoint &b)
            {
              if (a.x == b.x)
                return a.y < b.y;
              return a.x < b.x; });

  std::vector<ProjectedPoint> unique_points;
  unique_points.reserve(points.size());
  const float eps = 1e-6f;
  for (const ProjectedPoint &p : points)
  {
    if (!unique_points.empty())
    {
      const ProjectedPoint &u = unique_points.back();
      if (glm::abs(p.x - u.x) < eps && glm::abs(p.y - u.y) < eps)
        continue;
    }
    unique_points.push_back(p);
  }

  if (unique_points.size() < 3)
    return {};

  std::vector<ProjectedPoint> lower;
  for (const ProjectedPoint &p : unique_points)
  {
    while (lower.size() >= 2)
    {
      size_t n = lower.size();
      if (orient2D(lower[n - 2], lower[n - 1], p) > eps)
        break;
      lower.pop_back();
    }
    lower.push_back(p);
  }

  std::vector<ProjectedPoint> upper;
  for (size_t i = unique_points.size(); i > 0; --i)
  {
    const ProjectedPoint &p = unique_points[i - 1];
    while (upper.size() >= 2)
    {
      size_t n = upper.size();
      if (orient2D(upper[n - 2], upper[n - 1], p) > eps)
        break;
      upper.pop_back();
    }
    upper.push_back(p);
  }

  lower.pop_back();
  upper.pop_back();
  lower.insert(lower.end(), upper.begin(), upper.end());

  std::vector<int> hull_ids;
  hull_ids.reserve(lower.size());
  for (const ProjectedPoint &p : lower)
    hull_ids.push_back(p.point_id);

  return hull_ids;
}

static bool samePlane(const HullPlane &a, const glm::vec3 &normal, float d)
{
  const float normal_eps = 1e-4f;
  const float d_eps = 1e-4f;
  return glm::dot(a.normal, normal) > (1.0f - normal_eps) && glm::abs(a.d - d) < d_eps;
}

void convexHull(ConvexHullInfo info)
{
  std::vector<HullPoint> points;
  points.reserve(info.point_cloud.size());
  for (ushort index : info.point_cloud)
    points.push_back({index, info.all_vertices->at(index)});

  glm::vec3 centroid(0.0f);
  for (const HullPoint &point : points)
    centroid += point.pos;
  centroid /= static_cast<float>(points.size());

  const float eps = 1e-5f;
  std::vector<HullPlane> planes;

  const int count = static_cast<int>(points.size());
  for (int i = 0; i < count; ++i)
  {
    for (int j = i + 1; j < count; ++j)
    {
      for (int k = j + 1; k < count; ++k)
      {
        const glm::vec3 a = points[i].pos;
        const glm::vec3 b = points[j].pos;
        const glm::vec3 c = points[k].pos;

        glm::vec3 normal = glm::cross(b - a, c - a);
        float area2 = glm::length(normal);
        if (area2 < eps)
          continue;

        normal /= area2;

        int pos_count = 0;
        int neg_count = 0;
        for (int p = 0; p < count; ++p)
        {
          float side = glm::dot(normal, points[p].pos - a);
          if (side > eps)
            pos_count++;
          else if (side < -eps)
            neg_count++;
          if (pos_count > 0 && neg_count > 0)
            break;
        }

        if (pos_count > 0 && neg_count > 0)
          continue;

        if (glm::dot(normal, centroid - a) > 0.0f)
          normal = -normal;

        float d = -glm::dot(normal, a);

        bool exists = false;
        for (const HullPlane &plane : planes)
        {
          if (samePlane(plane, normal, d))
          {
            exists = true;
            break;
          }
        }
        if (exists)
          continue;

        HullPlane plane{normal, d, {}};
        for (int p = 0; p < count; ++p)
        {
          if (glm::abs(glm::dot(normal, points[p].pos) + d) <= eps * 4.0f)
            plane.point_ids.push_back(p);
        }

        if (plane.point_ids.size() >= 3)
          planes.push_back(plane);
      }
    }
  }

  std::vector<ushort> &hull_indices = *info.indices;
  for (const HullPlane &plane : planes)
  {
    glm::vec3 axis = glm::abs(plane.normal.x) < 0.9f ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 u = glm::normalize(glm::cross(axis, plane.normal));
    glm::vec3 v = glm::cross(plane.normal, u);

    std::vector<ProjectedPoint> projected;
    projected.reserve(plane.point_ids.size());
    for (int pid : plane.point_ids)
    {
      const glm::vec3 &pos = points[pid].pos;
      projected.push_back({glm::dot(pos, u), glm::dot(pos, v), pid});
    }

    std::vector<int> face = convexHull2D(projected);
    if (face.size() < 3)
      continue;

    for (size_t i = 1; i + 1 < face.size(); ++i)
    {
      hull_indices.push_back(points[face[0]].index);
      hull_indices.push_back(points[face[i]].index);
      hull_indices.push_back(points[face[i + 1]].index);
    }
  }
}