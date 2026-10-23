#include "gui.h"
#include "lib/gui/text_widget.h"
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"

GUI::GUI(Window &window) 
  : text_pipeline(window.ctx), 
  tick_rate_display(window, text_pipeline.atlas_details.glyph),
  render_fps_display(window, text_pipeline.atlas_details.glyph) 
{
  tick_rate_display.setPosition(50, 100);
  render_fps_display.setPosition(50, 50);
}

void GUI::render(SDL_GPUCommandBuffer *cmd, SDL_GPURenderPass *pass)
{
  RenderEvent<TextOverlayRenderObject> render_text_event = text_pipeline.startRender(cmd, pass);
  tick_rate_display.render(render_text_event);
  render_fps_display.render(render_text_event);
}