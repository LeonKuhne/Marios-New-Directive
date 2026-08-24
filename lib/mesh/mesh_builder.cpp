#include "mesh_builder.h"

#include <SDL3/SDL_log.h>

void MeshBuilder::build(std::vector<ushort> *indices, ushort offset) 
{
  for (const std::vector<ushort> &face_indices : faces)
  {
    // basic fan algorithm
    size_t num_triangles = face_indices.size() - 2;
    for (size_t i = 0; i < num_triangles; ++i)
    {
      indices->push_back(face_indices[0] + offset);
      indices->push_back(face_indices[i + 1] + offset);
      indices->push_back(face_indices[i + 2] + offset);
    }
  }
}