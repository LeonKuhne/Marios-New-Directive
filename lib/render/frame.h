#pragma once

#include <functional>
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include "window.h"
#include "context.h"

class Frame
{
  using Pass = std::function<void(SDL_GPUCommandBuffer*, SDL_GPURenderPass*)>;
  
  Context &ctx;
  std::vector<std::function<void()>> passes;
  SDL_GPUDepthStencilTargetInfo *depth_target = nullptr;
  uint16_t failed_swaps = 0;

  SDL_GPUColorTargetInfo pass_info = {
      .texture = NULL,
      .clear_color = {.r=0, .g=0, .b=0, .a=1},
      .load_op = SDL_GPU_LOADOP_CLEAR,
      .store_op = SDL_GPU_STOREOP_STORE,
      .cycle = false};

public:
  SDL_GPUCommandBuffer *cmd;

  Frame(Window &window) : ctx(window.ctx)
  {
    SDL_SetGPUSwapchainParameters(
      ctx.gpu, 
      ctx.window, 
      SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
      SDL_GPU_PRESENTMODE_IMMEDIATE
    );

    window.addListener([this, &window](uint w, uint h) {
      updateDepthTexture(w, h);
    });
    updateDepthTexture(window.width, window.height);
  };
  ~Frame();

  void addPass(const Pass &callback); 

  bool load();
  void publish();

private:
  void updateDepthTexture(uint width, uint height) {
    SDL_GPUTextureCreateInfo depth_desc = {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
        .usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
        .width = width,
        .height = height,
        .layer_count_or_depth = 1,
        .num_levels = 1};

    if (depth_target) {
      SDL_ReleaseGPUTexture(ctx.gpu, depth_target->texture);
      delete depth_target;
    }

    depth_target = new SDL_GPUDepthStencilTargetInfo{
      .texture = SDL_CreateGPUTexture(ctx.gpu, &depth_desc),
      .clear_depth = 1.0f,             // 1.0 is the farthest distance
      .load_op = SDL_GPU_LOADOP_CLEAR, // Clear the buffer every frame
      .store_op = SDL_GPU_STOREOP_STORE
    };
  }
};