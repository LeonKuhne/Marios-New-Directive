#pragma once

#include "lib/pbr/gpu_stored_object.h"
#include "lib/shapes/shape_data.h"
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>

class PlaneBuilder : public GPUStoredObject
{
private:
  inline static std::vector<glm::vec3> plane_vertices{
    {-0.5f, 0.0f,  0.5f}, // 0 front left
    { 0.5f, 0.0f,  0.5f}, // 1 front right
    {-0.5f, 0.0f, -0.5f}, // 2 back left
    { 0.5f, 0.0f, -0.5f}, // 3 back right
  };

  std::vector<glm::vec3>& all_vertices;

public:
  SDL_GPUBuffer *plane_index_buffer;
  std::vector<ushort> plane_indices;

  PlaneBuilder(SDL_GPUDevice *gpu, std::vector<glm::vec3> &all_vertices)
    : GPUStoredObject(gpu), all_vertices(all_vertices) {}

  void build(ShapeData &info) {
    info.mesh.indices = plane_indices;
    info.mesh.index_buffer = plane_index_buffer;
    info.mesh.all_vertices = &all_vertices;
  }

  ~PlaneBuilder() {
    /* TODO enable this
    SDL_ReleaseGPUBuffer(gpu, plane_index_buffer);
    */
  }

  void upload(SDL_GPUCopyPass *pass) override;

private:
  void planeGetIndices(std::vector<ushort> *indices, size_t offset);
};