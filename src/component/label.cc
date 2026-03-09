#include "label.hh"
#include "main.hh"

namespace miau {
  void label::update(float x, float y) {
    nvgFontFaceId(miau::vg, font);
    nvgFontSize(miau::vg, size);
    nvgTextLetterSpacing(miau::vg, space);
    nvgTextAlign(miau::vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

    float bounds[4];
    nvgTextBounds(miau::vg, x, y, text.c_str(), nullptr, bounds);
    _width  = bounds[2] - bounds[0];
    _height = bounds[3] - bounds[1];
  }

  void label::draw(float x, float y) {
    nvgFontFaceId(miau::vg, font);
    nvgFontSize(miau::vg, size);
    nvgTextLetterSpacing(miau::vg, space);
    nvgTextAlign(miau::vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgFillColor(
      miau::vg,
      nvgRGBA(
        color.r,
        color.g,
        color.b,
        color.a
      )
    );

    nvgText(miau::vg, x, y, text.c_str(), nullptr);
  }

  float label::width() const noexcept { return _width; }
  float label::height() const noexcept { return _height; }
}

