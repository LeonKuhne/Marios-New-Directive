#include "plane_builder.h"
#include "lib/gpu/transfer_vec.h"
#include "mesh_builder.h"

void PlaneBuilder::planeGetIndices(std::vector<ushort> *indices, size_t offset) {
  MeshBuilder *mesh = new MeshBuilder();
  mesh->addFace({0, 1, 3, 2});
  mesh->build(indices, offset);
}

void PlaneBuilder::upload(SDL_GPUCopyPass *pass) {
  ushort start_vertex = all_vertices.size();
  all_vertices.insert(all_vertices.end(), plane_vertices.begin(), plane_vertices.end());

  planeGetIndices(&plane_indices, start_vertex);

  // upload to gpu
  plane_index_buffer = vecToGPU<ushort>({gpu, pass, SDL_GPU_BUFFERUSAGE_INDEX, &plane_indices});
}