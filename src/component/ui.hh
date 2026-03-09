#ifndef _MIAU_COMPONENT_UI_HH
#define _MIAU_COMPONENT_UI_HH

#include <cstdint>

namespace miau {
  struct color {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 255;
  };

  struct edge {
    float top = 0.0f;
    float bottom = 0.0f;
    float left = 0.0f;
    float right = 0.0f;
  };

  enum class vertical {
    top,
    center,
    bottom,
  };

  enum class horizontal {
    left,
    center,
    right,
  };

  struct ui {
    virtual void update(float x, float y) {}
    virtual void draw(float x, float y) {}

    virtual float width() const noexcept = 0;
    virtual float height() const noexcept = 0;
  };
}

#endif
