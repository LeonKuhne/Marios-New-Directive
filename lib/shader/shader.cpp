#include "shader.h"

static uint8_t* shader_read_file(const char* path, size_t* out_code_size) {
  // construct full path
  const char* directory = "shaders/";
  const char* extension = ".spv";
  char full_path[strlen(directory) + strlen(path) + strlen(extension) + 1];
  strcpy(full_path, directory);
  strcat(full_path, path);
  strcat(full_path, extension);

  // open file
  size_t byte_size = 0;
  uint8_t* buffer = (uint8_t*) file_read(full_path, &byte_size);
  if (byte_size <= 0 || byte_size % 4 != 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid file size for %s: %zu, %s\n", path, byte_size, strerror(errno));
    return NULL;
  }

  // assert
  if (!buffer) {
    *out_code_size = 0;
    return NULL;
  }

  // submit
  *out_code_size = byte_size / 4;
  return buffer;
}

SDL_GPUShader* shader_load(const char* path, SDL_GPUDevice* gpu, SDL_GPUShaderStage stage, uint8_t num_uniforms) {
  size_t code_word_count = 0;
  uint8_t* code = shader_read_file(path, &code_word_count);

  // create gpu shader
  const SDL_GPUShaderCreateInfo shader_info = {
    .code_size = code_word_count * sizeof(uint32_t),
    .code = (const Uint8*) code,            
    .entrypoint = "main",
    .format = SDL_GPU_SHADERFORMAT_SPIRV,
    .stage = stage,
    .num_samplers = 0,
    .num_storage_textures = 0,
    .num_storage_buffers = 0,
    .num_uniform_buffers = num_uniforms,
  };
  SDL_GPUShader* shader = SDL_CreateGPUShader(gpu, &shader_info);
  free(code);

  // validate
  if (!shader) {
    SDL_Log("Failed to load shader: %s\n", SDL_GetError());
    return NULL;
  }

  return shader;
}