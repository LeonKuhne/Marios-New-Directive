#include "pbr_pipeline.h"
#include "lib/pbr/pbr_render_object.h"

PBRPipeline::PBRPipeline(Info info) : Pipeline(info.ctx, {
  .vertex_path="pbr.vert", 
  .fragment_path="material_pbr.frag", 
  .num_vert_uniforms=2, // projection/model/view/camPos + indices
  .num_vert_storage_buffers=1, 
  .num_frag_uniforms=3, // UBO + UBOparams + indices 
  .num_frag_samplers=8, 
  .num_frag_storage_buffers=2
}), state(info) 
{
  // initialize storage buffers
}

void PBRPipeline::render(RenderEvent<PBRRenderObject> &event, PBRRenderObject &obj)
{
  SDL_GPUCommandBuffer* cmd = event.cmd;
  SDL_GPURenderPass* render_pass = event.pass;

  struct PushConstants
  {
      int32_t meshIndex = 0;
      int32_t materialIndex = 0;
  };
  PushConstants constants;

  const Camera& camera = state.camera;
  const LightManager& light_manager = state.light_manager;
  const PBRVertices& pbr_vertices = state.pbr_vertices;
  const PBRMaterials& pbr_materials = state.pbr_materials;

  // setup ubo uniform data
  ubo_uniform_data.projection = camera.projection;
  ubo_uniform_data.model = obj.transform_matrix; 
  ubo_uniform_data.view = camera.view;
  ubo_uniform_data.camPos = camera.camera_pos;

  // setup ubo params uniform data
  ubo_params_uniform_data.lightCount = static_cast<int>(light_manager.lights.size());

  // submit vertex uniforms
  SDL_PushGPUVertexUniformData(cmd, 0, &ubo_uniform_data, sizeof(ubo_uniform_data));

  // submit fragment uniforms
  SDL_PushGPUFragmentUniformData(cmd, 0, &ubo_uniform_data, sizeof(ubo_uniform_data));
  SDL_PushGPUFragmentUniformData(cmd, 1, &ubo_params_uniform_data, sizeof(ubo_params_uniform_data));

  // todo read push constants from 
  SDL_PushGPUVertexUniformData(cmd, 1, &constants, sizeof(constants));
  SDL_PushGPUFragmentUniformData(cmd, 2, &constants, sizeof(constants));

  // submit vertex buffers
  SDL_GPUBufferBinding vertexBinding{.buffer=pbr_vertices.vertex_buffer, .offset=0};
  SDL_BindGPUVertexBuffers(render_pass, 0, &vertexBinding, 1);

  // submit index buffers
  SDL_GPUBufferBinding indexBinding{.buffer=obj.index_buffer, .offset=0};
  SDL_BindGPUIndexBuffer(render_pass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

  // bind storage buffers
  SDL_BindGPUVertexStorageBuffers(render_pass, 0, &pbr_vertices.mesh_shader_data_buffer, 1);
  SDL_BindGPUFragmentStorageBuffers(render_pass, 0, &pbr_materials.material_buffer, 1);
  SDL_BindGPUFragmentStorageBuffers(render_pass, 1, &light_manager.light_buffer, 1);

  SDL_GPUSampler *sampler = state.ctx.sampler;

  // bind texture
  SDL_GPUTextureSamplerBinding texture_bindings[5] = {
    {.texture=pbr_materials.default_texture, .sampler=sampler},
    {.texture=pbr_materials.default_texture, .sampler=sampler},
    {.texture=pbr_materials.default_texture, .sampler=sampler},
    {.texture=pbr_materials.default_texture, .sampler=sampler},
    {.texture=pbr_materials.default_texture, .sampler=sampler}
  };
  SDL_GPUTextureSamplerBinding cube_bindings[2] = {
    {.texture=pbr_materials.default_cube_texture, .sampler=sampler},
    {.texture=pbr_materials.default_cube_texture, .sampler=sampler},
  };
  SDL_GPUTextureSamplerBinding brdf_binding{pbr_materials.default_brdf_lut, sampler};

  SDL_BindGPUFragmentSamplers(render_pass, 0, texture_bindings, 5);
  SDL_BindGPUFragmentSamplers(render_pass, 5, cube_bindings, 2);
  SDL_BindGPUFragmentSamplers(render_pass, 7, &brdf_binding, 1);

  // draw
  SDL_DrawGPUIndexedPrimitives(render_pass, obj.num_indices, 1, 0, 0, 0);
}