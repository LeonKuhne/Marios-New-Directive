#include "line_pipeline.h"

void LinePipeline::render(Frame &frame, glm::vec3 start, glm::vec3 end)
{
  SDL_PushGPUVertexUniformData(frame.cmd, 0, &frame.view_projection, sizeof(glm::mat4));
  SDL_PushGPUVertexUniformData(frame.cmd, 1, &start, sizeof(glm::vec3));
  SDL_PushGPUVertexUniformData(frame.cmd, 2, &end, sizeof(glm::vec3));

  SDL_DrawGPUPrimitives(pass, 2, 1, 0, 0);
}