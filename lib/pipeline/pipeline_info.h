#pragma once

#include <SDL3/SDL_gpu.h>

class PipelineInfo 
{
public:
  const char *vertex_path;
  const char *fragment_path;
  bool has_vertex_input = true;
  uint8_t num_vert_uniforms = 0;
  uint8_t num_vert_storage_buffers = 0;
  uint8_t num_frag_uniforms = 0;
  uint8_t num_frag_samplers = 0;
  uint8_t num_frag_storage_buffers = 0;
  SDL_GPUPrimitiveType primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
  bool enable_depth_test = true;
  bool enable_depth_write = true;

  PipelineInfo operator+(const PipelineInfo& other) const {
    PipelineInfo result = *this;
    result.num_vert_uniforms += other.num_vert_uniforms;
    result.num_vert_storage_buffers += other.num_vert_storage_buffers;
    result.num_frag_uniforms += other.num_frag_uniforms;
    result.num_frag_samplers += other.num_frag_samplers;
    result.num_frag_storage_buffers += other.num_frag_storage_buffers;
    return result;
  };
};