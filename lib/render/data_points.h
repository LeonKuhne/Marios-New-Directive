#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>
#include <glm/glm.hpp>
#include <sys/types.h>
#include "lib/engine/config.h"
#include "lib/gpu/transfer_vec.h"
#include "lib/shapes/shape_data.h"
#include "lib/mesh/cloud.h"
#include "lib/mesh/cube.h"
#include "lib/mesh/plane.h"

class DataPoints
{
public:
  SDL_GPUDevice *gpu;

  SDL_GPUBuffer *vertex_buffer = nullptr;
  std::vector<glm::vec3> all_vertices;
  SDL_GPUBuffer *cube_index_buffer;
  std::vector<ushort> cube_indices;
  SDL_GPUBuffer *plane_index_buffer;
  std::vector<ushort> plane_indices;

  DataPoints(SDL_GPUDevice *gpu) : gpu(gpu)
  {
    // load shapes to gpu
    load([this](SDL_GPUCopyPass *pass) {
      // create poly point cloud
      cloudAddPoints(all_vertices, Config::point_cloud_size, Config::point_cloud_min_radius);

      // TODO is the get indices method here even useful since the upload methods call that

      // add cube
      cubeGetIndices(&cube_indices, all_vertices.size());
      cube_index_buffer = cubeUpload(this->gpu, pass, all_vertices);

      // add plane
      planeGetIndices(&plane_indices, all_vertices.size());
      plane_index_buffer = planeUpload(this->gpu, pass, all_vertices);

      return true; 
    });
  }

  // callback returns true if need to be updated
  void load(std::function<bool(SDL_GPUCopyPass *)> callback)
  {
    transfer(gpu, [this, callback](SDL_GPUCopyPass *pass) {
      if (!callback(pass))
        return;
      // update vertices
      if (vertex_buffer != nullptr) 
        SDL_ReleaseGPUBuffer(gpu, vertex_buffer);
      vertex_buffer = vecToGPU<glm::vec3>({gpu, pass, SDL_GPU_BUFFERUSAGE_VERTEX, &all_vertices}); });
  }

  ShapeData finishShape(ShapeData info, std::vector<ushort> indices, SDL_GPUBuffer *index_buffer)
  {
    info.mesh.indices = indices;
    info.mesh.index_buffer = index_buffer;
    info.mesh.all_vertices = &all_vertices;
    return info;
  }

  ShapeData finishCube(ShapeData cube)
  {
    return finishShape(cube, cube_indices, cube_index_buffer);
  }

  ShapeData finishPlane(ShapeData plane)
  {
    return finishShape(plane, plane_indices, plane_index_buffer);
  }
};