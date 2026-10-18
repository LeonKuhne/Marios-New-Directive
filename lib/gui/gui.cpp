#include "gui.h"
#include "lib/gui/text_widget.h"

GUI::GUI(Window &window) 
  : text_pipeline(window.ctx), 
  tick_rate_display(window, text_pipeline),
  render_fps_display(window, text_pipeline) 
{
  tick_rate_display.setPosition(50, 100);
  render_fps_display.setPosition(50, 50);
}

void GUI::render(SDL_GPUCommandBuffer *cmd, SDL_GPURenderPass *pass)
{
  text_pipeline.startRender(pass),
  tick_rate_display.render(cmd);
  render_fps_display.render(cmd);
}