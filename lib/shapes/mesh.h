#pragma once

#include <vector>
#include <sys/types.h>
#include <glm/glm.hpp>
#include <SDL3/SDL_gpu.h>

struct Mesh {
  std::vector<glm::vec3> *all_vertices;
  std::vector<ushort> indices;
  SDL_GPUBuffer *index_buffer;
};