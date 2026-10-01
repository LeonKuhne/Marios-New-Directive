#pragma once

#include <SDL3/SDL_gpu.h>
#include <glm/ext/matrix_float4x4.hpp>

struct RenderObject {
  glm::mat4 transform_matrix;
  SDL_GPUBuffer *index_buffer;
  Uint32 num_indices;
};