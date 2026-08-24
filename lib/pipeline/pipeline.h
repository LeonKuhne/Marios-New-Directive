#pragma once

#include <sys/types.h>
#include "lib/render/context.h"
#include "lib/shader/shader.h"

struct PipelineInfo
{
  const char *vertex_path;
  const char *fragment_path;
  ushort num_vert_uniforms;
  ushort num_frag_uniforms;
  bool has_vertex_input = true;
  SDL_GPUPrimitiveType primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
  bool enable_depth_test = true;
  bool enable_depth_write = true;
};

class Pipeline
{
protected:
  SDL_GPUGraphicsPipeline *pipeline;
  SDL_GPURenderPass *pass;

public:
  Pipeline(Context &ctx, PipelineInfo info);

  void start(SDL_GPURenderPass *pass);
};
