#include "frame.h"
#include <tracy/Tracy.hpp>

Frame::~Frame() {
  if (depth_target->texture)
    SDL_ReleaseGPUTexture(ctx.gpu, depth_target->texture);
  delete depth_target;

  if (failed_swaps > 0)
    SDL_Log("Swapchain texture failures: %d\n", failed_swaps);
}

bool Frame::load()
{
  // start frame
  {
    ZoneScopedN("Aquire GPU Command Buffer")
    cmd = SDL_AcquireGPUCommandBuffer(ctx.gpu);
  }

  {
    ZoneScopedN("Aquire GPU Swapchain Texture")
    SDL_AcquireGPUSwapchainTexture(cmd, ctx.window, &pass_info.texture, nullptr, nullptr);
    if (pass_info.texture == nullptr) {
      failed_swaps++;
      return false;
    }
  }

  return true;
}

void Frame::publish() {
  // run passes
  {
    ZoneScopedN("Render Passes")
    for (auto &pass : passes)
      pass();
  }

  // end frame
  SDL_SubmitGPUCommandBuffer(cmd);
}

void Frame::addPass(const Pass &callback) {
  passes.emplace_back([this, callback]() {
    SDL_GPURenderPass *pass = SDL_BeginGPURenderPass(cmd, &pass_info, 1, depth_target);
    callback(cmd, pass);
    SDL_EndGPURenderPass(pass);
  });
}