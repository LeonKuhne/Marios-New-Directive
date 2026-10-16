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
  static constexpr PipelineInfo base_info = {
    .vertex_path = "rect.vert",
    .fragment_path = "color.frag",
    .has_vertex_input = false,
    .num_vert_uniforms = 2,
    .num_vert_storage_buffers = 0,
    .num_frag_uniforms = 2,
    .num_frag_samplers = 0,
    .num_frag_storage_buffers = 0,
    .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    .enable_depth_test = false,
    .enable_depth_write = false
  };

public:
  OverlayPipeline(Context &ctx, PipelineInfo info) : Pipeline<T>(ctx, info + base_info) {};

  void render(SDL_GPUCommandBuffer *cmd, T &render_data) override {
    // shared data
    SDL_PushGPUVertexUniformData(cmd, 0, &render_data.shared, sizeof(render_data.shared));
    SDL_PushGPUFragmentUniformData(cmd, 0, &render_data.shared, sizeof(render_data.shared));

    // shader specific data
    SDL_PushGPUVertexUniformData(cmd, 1, &render_data.vert, sizeof(render_data.vert));
    SDL_PushGPUFragmentUniformData(cmd, 1, &render_data.frag, sizeof(render_data.frag));

    // draw
    SDL_DrawGPUPrimitives(this->render_pass, 6, 1, 0, 0);
  };
};