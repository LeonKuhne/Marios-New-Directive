#pragma once

#include "render_objects/overlay_render_object_base.h"
#include "lib/pipeline/pipeline.h"
#include "lib/render/context.h"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>
#include <glm/ext/vector_float2.hpp>
#include <concepts>

class Scene;

template<std::derived_from<OverlayRenderObjectBase> T>
class OverlayPipeline : public Pipeline<T> {
private:
  SDL_GPUGraphicsPipeline *pipeline;

public:
  OverlayPipeline(Context &ctx) : Pipeline<T>(ctx, {
    .vertex_path = "rect.vert",
    .fragment_path = "color.frag",
    .has_vertex_input = false,
    .num_vert_uniforms = 1,
    .num_vert_storage_buffers = 0,
    .num_frag_uniforms = 0,
    .num_frag_samplers = 0, // font atlas
    .num_frag_storage_buffers = 0,
    .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    .enable_depth_test = false,
    .enable_depth_write = false
  }) {};

  void render(SDL_GPUCommandBuffer *cmd, T &render_data) override {
    SDL_PushGPUVertexUniformData(cmd, 0, &render_data.vert, sizeof(render_data.vert));
    //SDL_PushGPUFragmentUniformData(cmd, 1, &render_data.frag, sizeof(render_data.frag));
    SDL_DrawGPUPrimitives(this->render_pass, 6, 1, 0, 0);
  };
};