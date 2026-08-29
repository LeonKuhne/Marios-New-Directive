#pragma once
#include "light.h"
#include <SDL3/SDL_gpu.h>

class LightManager {
private:
    int max_lights = 100;

public:
    SDL_GPUDevice *gpu;
    SDL_GPUBuffer *light_buffer = nullptr;
    std::vector<Light> lights;

    LightManager(SDL_GPUDevice *gpu);
    ~LightManager();

    void add(Light light);
    void updateLights();
};