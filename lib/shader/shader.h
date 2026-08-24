#pragma once

#include <SDL3/SDL.h>
#include "lib/file/file.h"

SDL_GPUShader* shader_load(const char* path, SDL_GPUDevice* gpu, SDL_GPUShaderStage stage, uint8_t num_uniforms);