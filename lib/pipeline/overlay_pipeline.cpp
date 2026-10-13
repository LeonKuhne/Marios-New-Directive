#include "overlay_pipeline.h"
#include "lib/scene/scene.h"

OverlayPipeline::OverlayPipeline(Context &ctx) : Pipeline(ctx, {
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
}) {}

void OverlayPipeline::render(Scene &scene) {
  OverlayData uniform_data = {
    .pos = {0.0f, 0.0f},
    .scale = {0.1f, 0.1f}
  };

  SDL_PushGPUVertexUniformData(scene.frame.cmd, 0, &uniform_data, sizeof(uniform_data));

  SDL_DrawGPUPrimitives(render_pass, 6, 1, 0, 0);
}