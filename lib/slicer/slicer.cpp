#include "slicer.h"

void Slicer::align(const SliceInfo &info)
{
  // transform slice into local coordinates
  SliceInfo slice{
    shape->inLocalSpace(info.a),
    shape->inLocalSpace(info.b),
    shape->inLocalSpace(info.c),
  };

  // create plane from slice
  plane = {
    slice.a,
    glm::normalize(glm::cross(slice.b - slice.a, slice.c - slice.a)),
  };
}

bool Slicer::slice(Shape *slices[2])
{
  bool success = false;

  // slice shape
  ShapeData shape_data[2];
  data_points.load([this, &success, &shape_data](SDL_GPUCopyPass *pass) {
     success = slice_pass(pass, shape_data);
     return success;
  });

  // create shapes
  if (!success)
    return false;
  slices[0] = new Shape(shape_data[0]);
  slices[1] = new Shape(shape_data[1]);
  return true;
}

bool Slicer::slice_pass(SDL_GPUCopyPass *pass, ShapeData shape_data[2])
{
  std::vector<ushort> side_indices[2];
  std::vector<IndexedPoint> cut_points;
  bool has_intersection = false;

  const std::vector<ushort> &shape_indices = shape->mesh.indices;
  for (size_t i = 0; i + 2 < shape_indices.size(); i += 3)
  {
    std::array<IndexedPoint, 3> triangle{};
    for (size_t j = 0; j < 3; ++j)
    {
      ushort index = shape_indices[i + j];
      const glm::vec3 &position = data_points.all_vertices.at(index);
      triangle[j] = IndexedPoint{
          .index = index,
          .position = position,
          .signed_distance = signedDistanceToPlane(position, plane),
      };
    }

    bool has_pos = false;
    bool has_neg = false;
    for (const IndexedPoint &point : triangle)
    {
      if (point.signed_distance > SLICE_EPSILON)
        has_pos = true;
      else if (point.signed_distance < -SLICE_EPSILON)
        has_neg = true;
    }
    if (has_pos && has_neg)
      has_intersection = true;

    std::vector<IndexedPoint> positive = clipTriangleToSide(triangle, plane, true, &data_points.all_vertices, cut_points);
    std::vector<IndexedPoint> negative = clipTriangleToSide(triangle, plane, false, &data_points.all_vertices, cut_points);

    appendTriangulatedPolygon(positive, side_indices[0]);
    appendTriangulatedPolygon(negative, side_indices[1]);
  }

  if (!has_intersection || side_indices[0].empty() || side_indices[1].empty())
    return false;

  std::vector<ushort> cap_hull = convexWrapCutPoints(cut_points, plane);
  if (cap_hull.size() >= 3)
  {
    for (size_t i = 1; i + 1 < cap_hull.size(); ++i)
    {
      side_indices[0].push_back(cap_hull[0]);
      side_indices[0].push_back(cap_hull[i + 1]);
      side_indices[0].push_back(cap_hull[i]);

      side_indices[1].push_back(cap_hull[0]);
      side_indices[1].push_back(cap_hull[i]);
      side_indices[1].push_back(cap_hull[i + 1]);
    }
  }

  const glm::vec3 source_linear_velocity = asGlmVec3(shape->body->getLinearVelocity());
  const glm::vec3 source_angular_velocity = asGlmVec3(shape->body->getAngularVelocity());

  for (int side = 0; side < 2; ++side)
  {
    shape_data[side] = shape->getShapeDataStarter();
    ShapeData &s = shape_data[side];
    s.mesh.all_vertices = &data_points.all_vertices;
    s.mesh.indices = side_indices[side];
    s.mesh.index_buffer = vecToGPU<ushort>({data_points.gpu, pass, SDL_GPU_BUFFERUSAGE_INDEX, &side_indices[side]});
    s.linear_velocity = source_linear_velocity;
    s.angular_velocity = source_angular_velocity;
    s.parent_transform = shape->body->getWorldTransform();
    s.parent_center_offset = shape->center_offset;
  }

  return true;
}