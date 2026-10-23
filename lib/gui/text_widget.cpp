#include "text_widget.h"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_surface.h>

// TODO: use buffers for pretty much everything here
// also get rid of the window width/height for the frag shader

TextWidget::TextWidget(Window &window, std::pair<Uint32, Uint32> glyph_size)
  : GPUTransferable(window.ctx.gpu),
  gpu_content_handler(window.ctx.gpu),
  glyph_size(glyph_size)
{
  setText("----");
  setPosition(0, 0);

  window.addListener([this](uint w, uint h) {
    updateWindowSize(w, h);
  });
  updateWindowSize(window.width, window.height);
}

void TextWidget::setPosition(uint x, uint y)
{
  render_data.vert.pos.x = x;
  render_data.vert.pos.y = y;
}

void TextWidget::updateWindowSize(uint w, uint h) 
{
  render_data.shared.window_size.width = w;
  render_data.shared.window_size.height = h;
}

void TextWidget::setText(const std::string &content) {
  this->content = content;
  render_data.frag.text_num_chars = content.size();
  render_data.vert.scale.width = glyph_size.first * content.size();
  render_data.vert.scale.height = glyph_size.second;
  markDirty();
}

void TextWidget::upload(SDL_GPUCopyPass *copy_pass) {
  std::vector<Uint32> chars;
  chars.reserve(content.size());
  for (unsigned char c : content)
    chars.push_back(static_cast<Uint32>(c));

  // todo instead of creating chars and all that see if you can just do content.data() below
  render_data.buffers.text = gpu_content_handler.upload({
    .pass = copy_pass,
    .data = chars.data(),
    .size = static_cast<Uint32>(chars.size() * sizeof(Uint32))
  });
}