#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>
#include "transfer.h"

template <typename T>
struct TransferVectorInfo
{
  SDL_GPUDevice *gpu;
  SDL_GPUCopyPass *pass;
  SDL_GPUBufferUsageFlags usage;
  const std::vector<T> *vec;
};

template <typename T>
SDL_GPUBuffer *vecToGPU(TransferVectorInfo<T> info)
{
  return toGPU<T>({info.gpu, info.pass, info.usage, info.vec->data(), (uint32_t)(info.vec->size() * sizeof(T))});
}