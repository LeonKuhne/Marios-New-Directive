#include "pbr_pipeline.h"
#include "lib/scene/scene.h"

void PBRPipeline::render(Scene &scene, Solid *solid)
{
  struct PushConstants
  {
      int32_t meshIndex = 0;
      int32_t materialIndex = 0;
  };
  PushConstants constants;

  const Camera& camera = scene.camera;
  const LightManager& light_manager = scene.light_manager;
  const PBRVertices& pbr_vertices = scene.pbr_vertices;
  const PBRMaterials& pbr_materials = scene.pbr_materials;

  // setup ubo uniform data
  ubo_uniform_data.projection = camera.projection;
  solid->getTransform(ubo_uniform_data.model);
  ubo_uniform_data.view = camera.view;
  ubo_uniform_data.camPos = camera.camera_pos;

  // setup ubo params uniform data
  ubo_params_uniform_data.lightCount = light_manager.lights.size();

  // submit vertex uniforms
  SDL_PushGPUVertexUniformData(scene.frame.cmd, 0, &ubo_uniform_data, sizeof(ubo_uniform_data));

  // submit fragment uniforms
  SDL_PushGPUFragmentUniformData(scene.frame.cmd, 0, &ubo_uniform_data, sizeof(ubo_uniform_data));
  SDL_PushGPUFragmentUniformData(scene.frame.cmd, 1, &ubo_params_uniform_data, sizeof(ubo_params_uniform_data));

  // todo read push constants from 
  SDL_PushGPUVertexUniformData(scene.frame.cmd, 1, &constants, sizeof(constants));
  SDL_PushGPUFragmentUniformData(scene.frame.cmd, 2, &constants, sizeof(constants));

  // submit vertex buffers
  SDL_GPUBufferBinding vertexBinding{pbr_vertices.vertex_buffer, 0};
  SDL_BindGPUVertexBuffers(render_pass, 0, &vertexBinding, 1);

  // submit index buffers
  SDL_GPUBufferBinding indexBinding{solid->mesh.index_buffer, 0};
  SDL_BindGPUIndexBuffer(render_pass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

  // bind storage buffers
  SDL_BindGPUVertexStorageBuffers(render_pass, 0, &pbr_vertices.mesh_shader_data_buffer, 1);
  SDL_BindGPUFragmentStorageBuffers(render_pass, 0, &pbr_materials.material_buffer, 1);
  SDL_BindGPUFragmentStorageBuffers(render_pass, 1, &light_manager.light_buffer, 1);

  // bind texture
  SDL_GPUTextureSamplerBinding texture_bindings[5] = {
    {pbr_materials.default_texture, sampler},
    {pbr_materials.default_texture, sampler},
    {pbr_materials.default_texture, sampler},
    {pbr_materials.default_texture, sampler},
    {pbr_materials.default_texture, sampler}
  };
  SDL_GPUTextureSamplerBinding cube_bindings[2] = {
    {pbr_materials.default_cube_texture, sampler},
    {pbr_materials.default_cube_texture, sampler},
  };
  SDL_GPUTextureSamplerBinding brdf_binding{pbr_materials.default_brdf_lut, sampler};

  SDL_BindGPUFragmentSamplers(render_pass, 0, texture_bindings, 5);
  SDL_BindGPUFragmentSamplers(render_pass, 5, cube_bindings, 2);
  SDL_BindGPUFragmentSamplers(render_pass, 7, &brdf_binding, 1);

  // draw
  SDL_DrawGPUIndexedPrimitives(render_pass, solid->mesh.indices.size(), 1, 0, 0, 0);
}