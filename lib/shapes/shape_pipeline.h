#pragma once

#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shape.h"
#include "lib/render/context.h"
#include "lib/render/frame.h"
#include "lib/pipeline/pipeline.h"

class ShapePipeline : public Pipeline
{
private:
  const SDL_FColor selected_color = {1.0f, 0.0f, 0.0f};

  struct
  {
    float view_projection[16];
    float transform_matrix[16];
    float center_offset[3];
    float _pad0;
    float scale[3];
    float _pad1;
  } uniform_data;

public:
  using Pipeline::Pipeline;
  ShapePipeline(Context &ctx) : Pipeline(ctx, {"mesh.vert", "color.frag", 2, 1}) {}

  void render(Frame &frame, Shape *shape);
};
