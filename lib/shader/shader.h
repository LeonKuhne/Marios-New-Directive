#pragma once

#include <SDL3/SDL.h>
#include "lib/file/file.h"

struct ShaderInfo {
    const char* path;
    SDL_GPUShaderStage stage;
    uint8_t num_uniforms;
    uint8_t num_samplers;
    uint8_t num_storage_buffers;
};

SDL_GPUShader* shader_load(SDL_GPUDevice* gpu, const ShaderInfo info);