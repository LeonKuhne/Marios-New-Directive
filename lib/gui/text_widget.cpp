#include "text_widget.h"
#include <SDL3/SDL_surface.h>
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"
#include <cstring>

TextWidget::TextWidget(Context &ctx, OverlayPipeline<TextOverlayRenderObject>& pipeline) : Widget(pipeline)
{
  memcpy(render_data.frag.text, "helowrld", 8);
  render_data.vert.pos[0] = 0.0f;
  render_data.vert.pos[1] = 0.1f;
  render_data.vert.scale[0] = 0.1f;
  render_data.vert.scale[1] = 0.1f;
};