#pragma once

#include <SDL3/SDL_gpu.h>
#include <functional>

template <typename T>
struct TransferInfo
{
  SDL_GPUDevice *gpu;
  SDL_GPUCopyPass *pass;
  SDL_GPUBufferUsageFlags usage;
  const T *data;
  uint32_t size;
};

struct TextureTransferInfo
{
  SDL_GPUDevice *gpu;
  SDL_GPUCopyPass *pass;
  const void *data;
  uint32_t width;
  uint32_t height;
};

struct TextureCubeTransferInfo
{
  SDL_GPUDevice *gpu;
  SDL_GPUCopyPass *pass;
  const void *data;
  uint32_t size;
};

void transfer(SDL_GPUDevice *gpu, std::function<void(SDL_GPUCopyPass*)> callback);

template <typename T> SDL_GPUBuffer *toGPU(TransferInfo<T> info)
{
  SDL_GPUBufferCreateInfo create_info{info.usage, info.size};
  SDL_GPUBuffer *buffer = SDL_CreateGPUBuffer(info.gpu, &create_info);

  SDL_GPUTransferBufferCreateInfo transferInfo{SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, info.size};
  SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(info.gpu, &transferInfo);
  void *shared_data = SDL_MapGPUTransferBuffer(info.gpu, transfer_buffer, false);
  memcpy(shared_data, info.data, info.size);
  SDL_UnmapGPUTransferBuffer(info.gpu, transfer_buffer);

  SDL_GPUTransferBufferLocation source{transfer_buffer, 0};
  SDL_GPUBufferRegion dest{buffer, 0, info.size};
  SDL_UploadToGPUBuffer(info.pass, &source, &dest, false);

  return buffer;
}

SDL_GPUTexture *textureToGPU(TextureTransferInfo info);
SDL_GPUTexture *textureCubeToGPU(TextureCubeTransferInfo info);