#include "shape_pipeline.h"

void ShapePipeline::render(Frame &frame, Shape *shape)
{
  // update view
  memcpy(uniform_data.view_projection, &frame.view_projection, sizeof(glm::mat4));
  shape->getTransform(uniform_data.transform_matrix);
  memcpy(uniform_data.center_offset, &shape->center_offset, sizeof(glm::vec3));
  memcpy(uniform_data.scale, &shape->scale, sizeof(glm::vec3));

  // submit vertex uniforms
  SDL_PushGPUVertexUniformData(frame.cmd, 0, &uniform_data, sizeof(uniform_data));

  // submit fragment uniforms
  SDL_FColor base_color = shape->selected ? selected_color : shape->color;
  float color[3] = {base_color.r, base_color.g, base_color.b};
  SDL_PushGPUFragmentUniformData(frame.cmd, 0, &color, sizeof(color));

  // submit vertex and index buffers
  SDL_GPUBufferBinding indexBinding{shape->mesh.index_buffer, 0};
  SDL_BindGPUIndexBuffer(pass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);
  SDL_GPUBufferBinding vertexBinding{frame.vertices, 0};
  SDL_BindGPUVertexBuffers(pass, 0, &vertexBinding, 1);

  // draw
  SDL_DrawGPUIndexedPrimitives(pass, shape->mesh.indices.size(), 1, 0, 0, 0);
}