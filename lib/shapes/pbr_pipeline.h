#pragma once

#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shape.h"
#include "lib/render/context.h"
#include "lib/render/frame.h"
#include "lib/pipeline/pipeline.h"

class PBRPipeline : public Pipeline
{
private:
  const SDL_FColor selected_color = {1.0f, 0.0f, 0.0f};

  struct
  {
    float projection[16];
    float model[16];
    float view[16];
    float camPos[3];
    float _pad0;
  } ubo_uniform_data;

  struct
  {
    int lightCount = 0;
    float exposure = 1.0f;
    float gamma = 2.2f;
    float prefilteredCubeMipLevels = 1.0f;
    float scaleIBLAmbient = 0.025f;
    float debugViewInputs = 0.0f;
    float debugViewEquation = 0.0f;
  } ubo_params_uniform_data;

  struct
  {
    int meshIndex = 0;
    int materialIndex = 0;
  } push_constants;

  SDL_GPUSampler *sampler;

public:
  using Pipeline::Pipeline;
  PBRPipeline(Context &ctx) : Pipeline(ctx, {
    .vertex_path="pbr.vert", 
    .fragment_path="material_pbr.frag", 
    .num_vert_uniforms=2, // projection/model/view/camPos + indices
    .num_frag_uniforms=3, // UBO + UBOparams + indices 
    .num_vert_storage_buffers=1, 
    .num_frag_samplers=8, 
    .num_frag_storage_buffers=2
  }) 
  {
    SDL_GPUSamplerCreateInfo sampler_info{};
    sampler_info.min_filter = SDL_GPU_FILTER_LINEAR;
    sampler_info.mag_filter = SDL_GPU_FILTER_LINEAR;
    sampler_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
    sampler_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    sampler_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    sampler_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    sampler = SDL_CreateGPUSampler(ctx.gpu, &sampler_info);
    if (!sampler)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create GPU sampler");
    }
  }
  void render(Frame &frame, Shape *shape);
};
