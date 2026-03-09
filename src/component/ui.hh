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

  namespace colors {
    inline constexpr color white = {255, 255, 255, 255};
    inline constexpr color black = {0, 0, 0, 255};
    inline constexpr color red = {255, 0, 0, 255};
    inline constexpr color green = {0, 255, 0, 255};
    inline constexpr color blue = {0, 0, 255, 255};
    inline constexpr color yellow = {255, 255, 0, 255};
    inline constexpr color cyan = {0, 255, 255, 255};
    inline constexpr color magenta = {255, 0, 255, 255};
    inline constexpr color transparent = {0, 0, 0, 0};
  }

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
