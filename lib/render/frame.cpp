#include "frame.h"


Frame::~Frame() {
  if (depth_target->texture)
    SDL_ReleaseGPUTexture(ctx.gpu, depth_target->texture);
  delete depth_target;

  if (failed_swaps > 0)
    SDL_Log("Swapchain texture failures: %d\n", failed_swaps);
}

void Frame::run() {
  // start frame
  cmd = SDL_AcquireGPUCommandBuffer(ctx.gpu);
  SDL_AcquireGPUSwapchainTexture(cmd, ctx.window, &pass_info.texture, nullptr, nullptr);
  if (pass_info.texture == nullptr) {
    failed_swaps++;
    return;
  }
  // run passes
  for (auto &pass : passes)
    pass();
  // end frame
  SDL_SubmitGPUCommandBuffer(cmd);
}

void Frame::addPass(std::function<void(Frame &, SDL_GPURenderPass *)> render) {
  passes.emplace_back([this, render]() {
    SDL_GPURenderPass *pass = SDL_BeginGPURenderPass(cmd, &pass_info, 1, depth_target);
    render(*this, pass);
    SDL_EndGPURenderPass(pass);
  });
}