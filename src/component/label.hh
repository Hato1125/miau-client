#ifndef _MIAU_COMPONENT_LABEL_HH
#define _MIAU_COMPONENT_LABEL_HH

#include <string>

#include "ui.hh"

namespace miau {
  class label : public ui {
  public:
    std::string text;
    float offset = 0.0f;
    float size = 17.0f;
    float space = 1.0f;
    int font = -1;
    color color;

    void update(float x, float y) override;
    void draw(float x, float y) override;

    float width() const noexcept override;
    float height() const noexcept override;

  private:
    float _width;
    float _height;
  };
}

#endif
