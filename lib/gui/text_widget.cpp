#include "text_widget.h"
#include <SDL3/SDL_surface.h>
#include "lib/pipeline/overlay/text_overlay_pipeline.h"

// TODO: use buffers for pretty much everything here
// also get rid of the window width/height for the frag shader

TextWidget::TextWidget(Window &window, TextOverlayPipeline& pipeline) : Widget(pipeline), GPUStoredObject(window.ctx.gpu)
{
  setText("hello world!");
  render_data.vert.pos.x = 50;
  render_data.vert.pos.y = 50;

  window.addListener([this](uint w, uint h) {
    updateWindowSize(w, h);
  });
  updateWindowSize(window.width, window.height);
};

void TextWidget::updateWindowSize(uint w, uint h) 
{
  render_data.shared.window_size.width = w;
  render_data.shared.window_size.height = h;
}

void TextWidget::setText(const std::string &content) {
  this->content = content;
  render_data.frag.text_num_chars = content.size();
  render_data.vert.scale.width = pipeline.atlas_details.glyph.width * content.size();
  render_data.vert.scale.height = pipeline.atlas_details.glyph.height;
};

void TextWidget::upload(SDL_GPUCopyPass *copy_pass) {
  std::vector<Uint32> chars;
  chars.reserve(content.size());
  for (unsigned char c : content)
    chars.push_back(static_cast<Uint32>(c));

  TransferInfo<Uint32> info{
    .gpu = gpu,
    .pass = copy_pass,
    .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
    .data = chars.data(),
    .size = static_cast<Uint32>(chars.size() * sizeof(Uint32))
  };

  render_data.buffers.text = toGPU(info);
};