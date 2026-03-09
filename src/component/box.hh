#ifndef _MIAU_COMPONENT_BOX_HH
#define _MIAU_COMPONENT_BOX_HH

#include <memory>
#include <vector>

#include "ui.hh"

namespace miau {
  class column : public ui {
  public:
    float rounded = 0.0f;
    float max_width = -1.0;
    float max_height = -1.0;
    bool fixed;
    edge padding;
    color color;
    horizontal align_horizontal = horizontal::left;
    vertical align_vertical = vertical::top;
    std::vector<std::shared_ptr<ui>> children;

    void update(float x, float y) override;
    void draw(float x, float y) override;

    float width() const noexcept override;
    float height() const noexcept override;

  private:
    float _width;
    float _height;

    float _childrens_width;
    float _childrens_height;
  };

  class row : public ui {
  public:
    float rounded = 0.0f;
    float max_width = -1.0f;
    float max_height = -1.0f;
    bool fixed;
    edge padding;
    color color;
    horizontal align_horizontal = horizontal::left;
    vertical align_vertical = vertical::top;
    std::vector<std::shared_ptr<ui>> children;

    void update(float x, float y) override;
    void draw(float x, float y) override;

    float width() const noexcept override;
    float height() const noexcept override;

  private:
    float _width;
    float _height;

    float _childrens_width;
    float _childrens_height;
  };
}

#endif
