#include "gui.h"
#include "lib/gui/text_widget.h"
#include <format>

GUI::GUI(Window &window) : text_pipeline(window.ctx), fps_display(window, text_pipeline) {}

void GUI::tick(float delta_time)
{
  float fps = 1.0f / delta_time;
  std::string fps_content = std::format("fps: {:3}", round(fps));
  fps_display.setText(fps_content);
}

void GUI::render(SDL_GPUCommandBuffer *cmd, SDL_GPURenderPass *pass)
{
  text_pipeline.startRender(pass),
  fps_display.render(cmd);
}