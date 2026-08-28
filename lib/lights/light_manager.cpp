#include "light_manager.h"
#include <SDL3/SDL_gpu.h>
#include "lib/gpu/transfer.h"

LightManager::LightManager(SDL_GPUDevice *gpu) : gpu(gpu) {}

void LightManager::add(Light light) 
{
  lights.push_back(light);
}

void LightManager::updateLights()
{
  transfer(this->gpu, [this](SDL_GPUCopyPass *pass) {
    if (light_buffer) {
      SDL_ReleaseGPUBuffer(this->gpu, light_buffer);
      light_buffer = nullptr;
    }
    light_buffer = toGPU<Light>({this->gpu, pass, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, lights.data(), static_cast<uint32_t>(lights.size() * sizeof(Light))});
    return true;
  });
}