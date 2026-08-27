#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>
#include <glm/glm.hpp>
#include <sys/types.h>
#include "lib/engine/config.h"
#include "lib/gpu/transfer_vec.h"
#include "lib/shapes/shape.h"
#include "lib/shapes/shape_data.h"
#include "lib/mesh/cloud.h"
#include "lib/mesh/cube.h"
#include "lib/mesh/plane.h"

struct MeshShaderDataBlock
{
  glm::mat4 matrix = glm::mat4(1.0f);
  glm::mat4 jointMatrix[128]{};
  uint32_t jointCount = 0;
};

struct ShaderMaterial
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
};

struct PBRVertex
{
  glm::vec3 pos;
  glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec2 uv0 = glm::vec2(0.0f);
  glm::vec2 uv1 = glm::vec2(0.0f);
  glm::uvec4 joint0 = glm::uvec4(0);
  glm::vec4 weight0 = glm::vec4(0.0f);
  glm::vec4 color0 = glm::vec4(1.0f);
};

class DataPoints
{
public:
  SDL_GPUDevice *gpu;

  SDL_GPUBuffer *vertex_buffer = nullptr;
  std::vector<glm::vec3> all_vertices;
  SDL_GPUBuffer *cube_index_buffer;
  std::vector<ushort> cube_indices;
  SDL_GPUBuffer *plane_index_buffer;
  std::vector<ushort> plane_indices;

  // pbr buffers
  std::vector<PBRVertex> pbr_vertices;
  SDL_GPUBuffer *pbr_vertex_buffer;
  SDL_GPUBuffer *mesh_shader_data_buffer;
  SDL_GPUBuffer *material_buffer;
  SDL_GPUTexture *default_texture;
  SDL_GPUTexture *default_cube_texture;
  SDL_GPUTexture *default_brdf_lut;

  DataPoints(SDL_GPUDevice *gpu) : gpu(gpu)
  {
    // load shapes to gpu
    load([this](SDL_GPUCopyPass *pass) {
      // create poly point cloud
      cloudAddPoints(all_vertices, Config::point_cloud_size, Config::point_cloud_min_radius);

      // pbr vertices
      pbr_vertices.resize(all_vertices.size());
      for (size_t i = 0; i < all_vertices.size(); i++)
      {
          pbr_vertices[i].pos = all_vertices[i];
      }
      pbr_vertex_buffer = vecToGPU<PBRVertex>({
          this->gpu,
          pass,
          SDL_GPU_BUFFERUSAGE_VERTEX,
          &pbr_vertices
      });

      // add cube
      cubeGetIndices(&cube_indices, all_vertices.size());
      cube_index_buffer = cubeUpload(this->gpu, pass, all_vertices);

      // add plane
      planeGetIndices(&plane_indices, all_vertices.size());
      plane_index_buffer = planeUpload(this->gpu, pass, all_vertices);

      // pbr animations
      MeshShaderDataBlock mesh_data{};
      mesh_shader_data_buffer = toGPU<MeshShaderDataBlock>({
        this->gpu,
        pass,
        SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        &mesh_data,
        sizeof(mesh_data)
      });

      // pbr materials
      ShaderMaterial material{};
      material_buffer = toGPU<ShaderMaterial>({
        this->gpu,
        pass,
        SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        &material,
        sizeof(material)
      });

      // pbr textures
      uint32_t white_pixel = 0xFFFFFFFF;
      default_texture = textureToGPU({
        this->gpu,
        pass,
        &white_pixel,
        1,
        1
      });

      // pbr cube texture
      uint32_t cube_pixel[6] = {
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF
      };
      default_cube_texture = textureCubeToGPU({
        this->gpu,
        pass,
        cube_pixel,
        sizeof(cube_pixel)
      });

      // pbr brdf lut
      uint32_t brdf_pixel = 0xFFFFFFFF;
      default_brdf_lut = textureToGPU({
        this->gpu,
        pass,
        &brdf_pixel,
        1,
        1
      });

      return true; 
    });
  }

  // callback returns true if need to be updated
  void load(std::function<bool(SDL_GPUCopyPass *)> callback)
  {
    transfer(gpu, [this, callback](SDL_GPUCopyPass *pass) {
      if (!callback(pass))
        return;
      // update vertices
      if (vertex_buffer != nullptr) 
        SDL_ReleaseGPUBuffer(gpu, vertex_buffer);
      vertex_buffer = vecToGPU<glm::vec3>({gpu, pass, SDL_GPU_BUFFERUSAGE_VERTEX, &all_vertices}); });
  }

  void finishShape(ShapeData& info, std::vector<ushort> indices, SDL_GPUBuffer *index_buffer)
  {
    info.mesh.indices = indices;
    info.mesh.index_buffer = index_buffer;
    info.mesh.all_vertices = &all_vertices;
  }

  void finishCube(ShapeData& cube)
  {
    finishShape(cube, cube_indices, cube_index_buffer);
  }

  void finishPlane(ShapeData& plane)
  {
    finishShape(plane, plane_indices, plane_index_buffer);
  }
};