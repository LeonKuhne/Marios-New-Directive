#include "pbr_pipeline.h"

void PBRPipeline::render(Frame &frame, Shape *shape)
{
  // setup push constants
  struct PushConstants
  {
      int32_t meshIndex = 0;
      int32_t materialIndex = 0;
  };
  PushConstants indices;

  // setup ubo uniform data
  memcpy(ubo_uniform_data.projection, &frame.camera.projection, sizeof(glm::mat4));
  shape->getTransform(ubo_uniform_data.model);
  memcpy(ubo_uniform_data.view, &frame.camera.view, sizeof(glm::mat4));
  memcpy(ubo_uniform_data.camPos, &frame.camera.camera_pos, sizeof(glm::vec3));

  // submit vertex uniforms
  SDL_PushGPUVertexUniformData(frame.cmd, 0, &ubo_uniform_data, sizeof(ubo_uniform_data));

  // submit fragment uniforms
  SDL_PushGPUFragmentUniformData(frame.cmd, 0, &ubo_uniform_data, sizeof(ubo_uniform_data));
  SDL_PushGPUFragmentUniformData(frame.cmd, 1, &ubo_params_uniform_data, sizeof(ubo_params_uniform_data));
  SDL_PushGPUFragmentUniformData(frame.cmd, 2, &indices, sizeof(indices));

  // submit vertex buffers
  // TODO switch to pbr
  //SDL_GPUBufferBinding vertexBinding{frame.data_points.pbr_vertex_buffer, 0};
  SDL_GPUBufferBinding vertexBinding{
    frame.data_points.vertex_buffer,
    0
  };
  SDL_BindGPUVertexBuffers(pass, 0, &vertexBinding, 1);
  SDL_PushGPUVertexUniformData(frame.cmd, 1, &indices, sizeof(indices));


SDL_BindGPUVertexBuffers(pass, 0, &vertexBinding, 1);

  // submit index buffers
  SDL_GPUBufferBinding indexBinding{shape->mesh.index_buffer, 0};
  SDL_BindGPUIndexBuffer(pass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

  // bind storage buffers
  SDL_BindGPUVertexStorageBuffers(pass, 0, &frame.data_points.mesh_shader_data_buffer, 1);
  SDL_BindGPUFragmentStorageBuffers(pass, 8, &frame.data_points.material_buffer, 1);

  // bind texture
  SDL_GPUTextureSamplerBinding texture_bindings[5] = {
    {frame.data_points.default_texture, sampler},
    {frame.data_points.default_texture, sampler},
    {frame.data_points.default_texture, sampler},
    {frame.data_points.default_texture, sampler},
    {frame.data_points.default_texture, sampler}
  };
  SDL_BindGPUFragmentSamplers(pass, 0, texture_bindings, 5);

  // bind cube texture
  SDL_GPUTextureSamplerBinding cube_bindings[3] = {
    {frame.data_points.default_cube_texture, sampler},
    {frame.data_points.default_cube_texture, sampler},
    {frame.data_points.default_cube_texture, sampler}
  };
  SDL_BindGPUFragmentSamplers(pass, 5, cube_bindings, 3);

  // bind brdf lut texture
  SDL_GPUTextureSamplerBinding brdf_binding{frame.data_points.default_brdf_lut, sampler};
  SDL_BindGPUFragmentSamplers(pass, 7, &brdf_binding, 1);

  // draw
  SDL_DrawGPUIndexedPrimitives(pass, shape->mesh.indices.size(), 1, 0, 0, 0);
}