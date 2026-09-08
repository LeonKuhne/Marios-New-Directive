#pragma once

#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include <vector>
#include <sys/types.h>
#include "lib/gpu/transfer_vec.h"
#include "lib/pbr/gpu_stored_object.h"

class PBRVertices : public GPUStoredObject
{
public:
  struct VertexData
  {
    glm::vec3 pos;
    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec2 uv0 = glm::vec2(0.0f);
    glm::vec2 uv1 = glm::vec2(0.0f);
    glm::uvec4 joint0 = glm::uvec4(0);
    glm::vec4 weight0 = glm::vec4(0.0f);
    glm::vec4 color0 = glm::vec4(1.0f);
  };

private:
  struct AnimationData
  {
    glm::mat4 matrix = glm::mat4(1.0f);
    glm::mat4 jointMatrix[128]{};
    uint32_t jointCount = 0;
  };

  std::vector<glm::vec3>& points;
  std::vector<VertexData> vertices;

public:
  SDL_GPUBuffer *vertex_buffer;
  SDL_GPUBuffer *mesh_shader_data_buffer;

  PBRVertices(SDL_GPUDevice *gpu, std::vector<glm::vec3>& points) : GPUStoredObject(gpu), points(points) {}

  void upload(SDL_GPUCopyPass *pass) override
  {
    // vertices
    vertices.resize(points.size());
    for (size_t i = 0; i < points.size(); i++)
    {
        vertices[i].pos = points[i];
    }
    vertex_buffer = vecToGPU<VertexData>({gpu, pass, SDL_GPU_BUFFERUSAGE_VERTEX, &vertices});

    // animations
    AnimationData mesh_data{};
    mesh_shader_data_buffer = toGPU<AnimationData>({gpu, pass, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, &mesh_data, sizeof(mesh_data)});
  }

  ~PBRVertices()
  {
    /* TODO enable this
    SDL_ReleaseGPUBuffer(gpu, vertex_buffer);
    SDL_ReleaseGPUBuffer(gpu, mesh_shader_data_buffer);
    */
  }
};

