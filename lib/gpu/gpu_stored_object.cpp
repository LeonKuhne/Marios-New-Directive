#include "gpu_stored_object.h"

SDL_GPUBuffer* GPUStoredObject::upload(UploadInfo info)
{
  if (info.size > capacity)
  {
    capacity = info.size;
    recreateBuffers();
  }

  dest.size = info.size;
  void *shared_data = SDL_MapGPUTransferBuffer(gpu, source.transfer_buffer, true);
  memcpy(shared_data, info.data, info.size);
  SDL_UnmapGPUTransferBuffer(gpu, source.transfer_buffer);
  SDL_UploadToGPUBuffer(info.pass, &source, &dest, true);

  return dest.buffer;
}

void GPUStoredObject::recreateBuffers()
{
  destroyBuffers();
  createBuffers();
}

void GPUStoredObject::createBuffers()
{
  SDL_GPUTransferBufferCreateInfo transferInfo{.usage=SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size=capacity};
  source.transfer_buffer = SDL_CreateGPUTransferBuffer(gpu, &transferInfo);
  SDL_GPUBufferCreateInfo create_info{.usage=SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, .size=capacity};
  dest.buffer = SDL_CreateGPUBuffer(gpu, &create_info);
}

void GPUStoredObject::destroyBuffers()
{
  SDL_ReleaseGPUBuffer(gpu, dest.buffer);
  SDL_ReleaseGPUTransferBuffer(gpu, source.transfer_buffer);
}