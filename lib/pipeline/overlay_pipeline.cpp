#include "overlay_pipeline.h"

OverlayPipeline::OverlayPipeline(Context &ctx) : Pipeline(ctx, {
  .vertex_path = "fullscreen.vert",
  .fragment_path = "color.frag",
  .has_vertex_input = false,
  .num_vert_uniforms = 0,
  .num_vert_storage_buffers = 0,
  .num_frag_uniforms = 0,
  .num_frag_samplers = 0, // font atlas
  .num_frag_storage_buffers = 0,
  .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
  .enable_depth_test = false,
  .enable_depth_write = false
}) {}

void OverlayPipeline::render(Scene &scene) {
  // todo pass fps text to print and position/size of text on gui




  SDL_DrawGPUPrimitives(render_pass, 6, 1, 0, 0);
}