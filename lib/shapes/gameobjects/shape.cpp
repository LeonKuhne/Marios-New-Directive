#include "shape.h"
#include <glm/gtc/type_ptr.hpp>
#include <unordered_set>
#include <optional>
#include "lib/shapes/util.h"
#include "lib/util/vertex.h"

Shape::Shape(const ShapeData &info)
    : color(info.color),
      mesh(info.mesh),
      scale(info.scale),
      is_visible(info.is_visible)
{
  std::unordered_set<ushort> unique_indices = vec2set<ushort>(info.mesh.indices);

  // compute centered vertices
  centered_vertices = subSet(unique_indices, info.mesh.all_vertices);
  glm::vec3 mesh_center = vecAvg(centered_vertices);
  for (glm::vec3 &vertex : centered_vertices)
    vertex -= mesh_center;
  center_offset = mesh_center;

  // set transform
  transform.setIdentity();
  transform.setRotation(asBtQuaternion(info.rotation));
  btVector3 pos = asBtVector3(info.pos);
  if (info.parent_transform.has_value())
  {
    glm::vec3 parent_center = info.parent_center_offset.value_or(glm::vec3(0.0f));
    glm::vec3 center_delta = (mesh_center - parent_center) * scale;
    pos = (*info.parent_transform) * asBtVector3(center_delta);
  }
  transform.setOrigin(pos);
}