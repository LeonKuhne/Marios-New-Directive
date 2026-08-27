#include "transfer.h"

void transfer(SDL_GPUDevice *gpu, std::function<void(SDL_GPUCopyPass*)> callback)
{
  SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(gpu);
  SDL_GPUCopyPass *pass = SDL_BeginGPUCopyPass(cmd);
  callback(pass);
  SDL_EndGPUCopyPass(pass);
  SDL_SubmitGPUCommandBuffer(cmd);
}

SDL_GPUTexture *textureToGPU(TextureTransferInfo info)
{
  SDL_GPUTextureCreateInfo texture_info{};
  texture_info.type = SDL_GPU_TEXTURETYPE_2D;
  texture_info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
  texture_info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
  texture_info.width = info.width;
  texture_info.height = info.height;
  texture_info.layer_count_or_depth = 1;
  texture_info.num_levels = 1;

  SDL_GPUTexture *texture =
      SDL_CreateGPUTexture(info.gpu, &texture_info);

  SDL_GPUTransferBufferCreateInfo transfer_info{};
  transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
  transfer_info.size = info.width * info.height * 4;

  SDL_GPUTransferBuffer *transfer_buffer =
      SDL_CreateGPUTransferBuffer(info.gpu, &transfer_info);

  void *mapped =
      SDL_MapGPUTransferBuffer(info.gpu, transfer_buffer, false);

  memcpy(mapped, info.data, transfer_info.size);

  SDL_UnmapGPUTransferBuffer(info.gpu, transfer_buffer);

  SDL_GPUTextureTransferInfo source{};
  source.transfer_buffer = transfer_buffer;
  source.offset = 0;

  SDL_GPUTextureRegion destination{};
  destination.texture = texture;
  destination.w = info.width;
  destination.h = info.height;
  destination.d = 1;

  SDL_UploadToGPUTexture(
      info.pass,
      &source,
      &destination,
      false
  );

  return texture;
}

SDL_GPUTexture *textureCubeToGPU(TextureCubeTransferInfo info)
{
  SDL_GPUTextureCreateInfo texture_info{};
  texture_info.type = SDL_GPU_TEXTURETYPE_CUBE;
  texture_info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
  texture_info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
  texture_info.width = 1;
  texture_info.height = 1;
  texture_info.layer_count_or_depth = 6;
  texture_info.num_levels = 1;

  SDL_GPUTexture *texture =
      SDL_CreateGPUTexture(info.gpu, &texture_info);

  SDL_GPUTransferBufferCreateInfo transfer_info{};
  transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
  transfer_info.size = info.size;

  SDL_GPUTransferBuffer *transfer_buffer =
      SDL_CreateGPUTransferBuffer(info.gpu, &transfer_info);

  void *mapped =
      SDL_MapGPUTransferBuffer(info.gpu, transfer_buffer, false);

  memcpy(mapped, info.data, info.size);

  SDL_UnmapGPUTransferBuffer(info.gpu, transfer_buffer);

  for (uint32_t i = 0; i < 6; i++)
  {
    SDL_GPUTextureTransferInfo source{};
    source.transfer_buffer = transfer_buffer;
    source.offset = i * 4;
    source.pixels_per_row = 1;
    source.rows_per_layer = 1;

    SDL_GPUTextureRegion destination{};
    destination.texture = texture;
    destination.mip_level = 0;
    destination.layer = i;
    destination.x = 0;
    destination.y = 0;
    destination.z = 0;
    destination.w = 1;
    destination.h = 1;
    destination.d = 1;

    SDL_UploadToGPUTexture(
      info.pass,
      &source,
      &destination,
      false
    );
  }

  return texture;
}