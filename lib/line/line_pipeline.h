#pragma once

#include <glm/glm.hpp>
#include "lib/pipeline/pipeline.h"
#include "lib/render/frame.h"

class LinePipeline : public Pipeline
{
public:
  LinePipeline(Context &ctx) : Pipeline(ctx, {"line.vert", "line.frag", 3, 0, false, SDL_GPU_PRIMITIVETYPE_LINELIST, false, false}) {}

  void render(Frame &frame, glm::vec3 start, glm::vec3 end);
};