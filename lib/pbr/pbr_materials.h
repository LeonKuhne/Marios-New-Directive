#pragma once

#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include <sys/types.h>
#include "lib/gpu/transfer.h"
#include "lib/pbr/gpu_stored_object.h"

class PBRMaterials : public GPUStoredObject
{

private:
  struct MaterialData // grouped to match shader 16 block layout
  {
    glm::vec4 baseColorFactor = glm::vec4(1.0f);
    glm::vec4 emissiveFactor = glm::vec4(0.0f);
    glm::vec4 diffuseFactor = glm::vec4(1.0f);
    glm::vec4 specularFactor = glm::vec4(1.0f);

    float workflow = 0.0f;
    int32_t baseColorTextureSet = -1;
    int32_t physicalDescriptorTextureSet = -1;
    int32_t normalTextureSet = -1;

    int32_t occlusionTextureSet = -1;
    int32_t emissiveTextureSet = -1;
    float metallicFactor = 0.0f;
    float roughnessFactor = 1.0f;

    float alphaMask = 0.0f;
    float alphaMaskCutoff = 0.5f;
    float emissiveStrength = 1.0f;
    int _pad;
  };

public:
  SDL_GPUBuffer *material_buffer;
  SDL_GPUTexture *default_texture;
  SDL_GPUTexture *default_cube_texture;
  SDL_GPUTexture *default_brdf_lut;

  PBRMaterials(SDL_GPUDevice *gpu) : GPUStoredObject(gpu) {}

  void upload(SDL_GPUCopyPass *pass) override
  {
    // pbr materials
    MaterialData material{};
    material_buffer = toGPU<MaterialData>({gpu, pass, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, &material, sizeof(material)});

    // pbr textures
    uint32_t white_pixel = 0xFFFFFFFF;
    default_texture = textureToGPU({gpu, pass, &white_pixel, 1, 1});

    // pbr cube texture
    uint32_t cube_pixel[6] = {
      0xFFFFFFFF,
      0xFFFFFFFF,
      0xFFFFFFFF,
      0xFFFFFFFF,
      0xFFFFFFFF,
      0xFFFFFFFF
    };
    default_cube_texture = textureCubeToGPU({gpu, pass, cube_pixel, sizeof(cube_pixel)});

    // pbr brdf lut
    uint32_t brdf_pixel = 0xFFFFFFFF;
    default_brdf_lut = textureToGPU({gpu, pass, &brdf_pixel, 1, 1});
  }

  ~PBRMaterials()
  {
    /* TODO enable this
    SDL_ReleaseGPUBuffer(gpu, material_buffer);
    SDL_ReleaseGPUTexture(gpu, default_texture);
    SDL_ReleaseGPUTexture(gpu, default_cube_texture);
    SDL_ReleaseGPUTexture(gpu, default_brdf_lut);
    */
  }
};
