#include "transfer.h"

void transfer(SDL_GPUDevice *gpu, std::function<void(SDL_GPUCopyPass*)> callback)
{
  SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(gpu);
  SDL_GPUCopyPass *pass = SDL_BeginGPUCopyPass(cmd);
  callback(pass);
  SDL_EndGPUCopyPass(pass);
  SDL_SubmitGPUCommandBuffer(cmd);
}