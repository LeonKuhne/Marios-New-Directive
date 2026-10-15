#pragma once

#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/lights/light_manager.h"
#include "lib/pbr/pbr_materials.h"
#include "lib/pbr/pbr_vertices.h"
#include "lib/pipeline/pipeline.h"
#include "lib/pbr/pbr_render_object.h"
#include "lib/scene/camera.h"

class Scene;
class Shape;

class PBRPipeline : public Pipeline<PBRRenderObject>
{
private:
  const SDL_FColor selected_color = {1.0f, 0.0f, 0.0f};

  struct
  {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    glm::vec3 camPos;
    float _pad0;
  } ubo_uniform_data;

  struct
  {
    int lightCount = 0;
    float exposure = 1.0f;
    float gamma = 2.2f;
    float prefilteredCubeMipLevels = 1.0f;
    float scaleIBLAmbient = 0.025f;
    float debugViewInputs = 0.0f;
    float debugViewEquation = 0.0f;
  } ubo_params_uniform_data;

  struct
  {
    int meshIndex = 0;
    int materialIndex = 0;
  } push_constants;

public:
  struct Info
  {
    Context &ctx;
    Camera &camera;
    LightManager &light_manager;
    PBRVertices &pbr_vertices;
    PBRMaterials &pbr_materials;
  };

  Info state;

  PBRPipeline(Info info);
  void render(SDL_GPUCommandBuffer *cmd, PBRRenderObject &obj) override;
};
