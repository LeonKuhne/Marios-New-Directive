#pragma once

#include "lib/gui/widget.h"
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"
#include "lib/pipeline/overlay/text_overlay_pipeline.h"
#include "lib/render/context.h"

class TextWidget : public Widget<TextOverlayRenderObject>
{
public:
  TextWidget(Context &ctx, TextOverlayPipeline& pipeline);
};