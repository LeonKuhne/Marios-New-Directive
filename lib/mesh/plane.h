#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>

static std::vector<glm::vec3> plane_vertices{
  {-0.5f, 0.0f,  0.5f}, // 0 front left
  { 0.5f, 0.0f,  0.5f}, // 1 front right
  {-0.5f, 0.0f, -0.5f}, // 2 back left
  { 0.5f, 0.0f, -0.5f}, // 3 back right
};

void planeGetIndices(std::vector<ushort> *indices, size_t offset);
SDL_GPUBuffer *planeUpload(SDL_GPUDevice *gpu, SDL_GPUCopyPass *pass, std::vector<glm::vec3> &all_vertices);