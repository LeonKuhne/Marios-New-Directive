#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include "lib/gpu/transfer_vec.h"
#include "mesh_builder.h"

static std::vector<glm::vec3> cube_vertices{
  {-0.5f, -0.5f,  0.5f}, // 0 front bot left
  { 0.5f, -0.5f,  0.5f}, // 1 front bot right
  {-0.5f,  0.5f,  0.5f}, // 2 front top left
  { 0.5f,  0.5f,  0.5f}, // 3 front top right
  {-0.5f, -0.5f, -0.5f}, // 4 back bot left
  { 0.5f, -0.5f, -0.5f}, // 5 back bot right
  {-0.5f,  0.5f, -0.5f}, // 6 back top left
  { 0.5f,  0.5f, -0.5f}  // 7 back top right
};

void cubeGetIndices(std::vector<ushort> *indices, size_t offset);
SDL_GPUBuffer *cubeUpload(SDL_GPUDevice *gpu, SDL_GPUCopyPass *pass, std::vector<glm::vec3> &all_vertices);