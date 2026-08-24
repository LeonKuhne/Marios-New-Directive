#include "cube.h"

void cubeGetIndices(std::vector<ushort> *indices, size_t offset) {
  MeshBuilder *mesh = new MeshBuilder();
  mesh->addFace({0, 1, 3, 2}); // front
  mesh->addFace({4, 6, 7, 5}); // back
  mesh->addFace({0, 4, 5, 1}); // bottom
  mesh->addFace({2, 3, 7, 6}); // top
  mesh->addFace({1, 5, 7, 3}); // right
  mesh->addFace({0, 2, 6, 4}); // left
  mesh->build(indices, offset);
}

SDL_GPUBuffer *cubeUpload(SDL_GPUDevice *gpu, SDL_GPUCopyPass *pass, std::vector<glm::vec3> &all_vertices)
{
  ushort start_vertex = all_vertices.size();
  auto &vertices = cube_vertices;
  all_vertices.insert(all_vertices.end(), vertices.begin(), vertices.end());

  std::vector<ushort> indices;
  cubeGetIndices(&indices, start_vertex);

  // upload to gpu
  return vecToGPU<ushort>({gpu, pass, SDL_GPU_BUFFERUSAGE_INDEX, &indices});
}