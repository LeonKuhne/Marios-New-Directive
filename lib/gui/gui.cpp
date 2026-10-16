#include "gui.h"
#include "lib/gui/text_widget.h"

GUI::GUI(Window &window) : text_pipeline(window.ctx), fps_display(window, text_pipeline) {}

void GUI::render(SDL_GPUCommandBuffer *cmd, SDL_GPURenderPass *pass)
{
  text_pipeline.startRender(pass),
  fps_display.render(cmd);
}