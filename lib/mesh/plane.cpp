#include "plane.h"
#include "lib/gpu/transfer_vec.h"
#include "mesh_builder.h"

void planeGetIndices(std::vector<ushort> *indices, size_t offset) {
  MeshBuilder *mesh = new MeshBuilder();
  mesh->addFace({0, 1, 3, 2});
  mesh->build(indices, offset);
}

SDL_GPUBuffer *planeUpload(SDL_GPUDevice *gpu, SDL_GPUCopyPass *pass, std::vector<glm::vec3> &all_vertices)
{
  ushort start_vertex = all_vertices.size();
  auto &vertices = plane_vertices;
  all_vertices.insert(all_vertices.end(), vertices.begin(), vertices.end());

  std::vector<ushort> indices;
  planeGetIndices(&indices, start_vertex);

  // upload to gpu
  return vecToGPU<ushort>({gpu, pass, SDL_GPU_BUFFERUSAGE_INDEX, &indices});
}