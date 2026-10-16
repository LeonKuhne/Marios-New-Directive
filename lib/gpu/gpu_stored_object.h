#pragma once

#include <SDL3/SDL_gpu.h>

class GPUStoredObject
{
public:
  struct UploadInfo
  {
    SDL_GPUCopyPass *pass;
    const void *data;
    Uint32 size;
  };

private:
  SDL_GPUDevice *gpu;
  Uint32 capacity = 0;

  SDL_GPUTransferBufferLocation source{.offset = 0};
  SDL_GPUBufferRegion dest{.offset = 0};

public:
  GPUStoredObject(SDL_GPUDevice *gpu) :gpu(gpu) {}

  ~GPUStoredObject() { destroyBuffers(); }

  SDL_GPUBuffer* upload(UploadInfo info);

private:
  void createBuffers();
  void recreateBuffers();
  void destroyBuffers();
};