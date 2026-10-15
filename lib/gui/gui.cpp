#include "gui.h"
#include "lib/gui/text_widget.h"

GUI::GUI(Context &ctx) : text_pipeline(ctx), fps_display(ctx, text_pipeline) {}

void GUI::render(SDL_GPUCommandBuffer *cmd, SDL_GPURenderPass *pass)
{
  text_pipeline.startRender(pass),
  fps_display.render(cmd);
}