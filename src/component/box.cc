#include <print>

#include "main.hh"
#include "box.hh"

namespace miau {
  void column::update(float x, float y) {
    _width = 0;
    _height = 0;
    _childrens_width = 0;
    _childrens_height = 0;

    for (auto& child : children) {
      child->update(x, y + _height);
      _width = std::max(child->width(), _width);
      _height += child->height() + spacing;
    }
    _height -= spacing;

    _childrens_width = _width;
    _childrens_height = _height;

    if (min_width > _width) _width = min_width;
    if (min_height > _height) _height = min_height;

    if (fixed) {
      if (max_width >= 0) _width = max_width;
      if (max_height >= 0) _height = max_height;
    } else {
      _width += padding.left + padding.right;
      _height += padding.top + padding.bottom;
      if (max_width >= 0 && _width > max_width) _width = max_width;
      if (max_height >= 0 && _height > max_height) _height = max_height;
    }
  }

  void column::draw(float x, float y) {
    nvgBeginPath(miau::vg);
    nvgRoundedRect(
      miau::vg,
      x,
      y,
      _width,
      _height,
      rounded
    );
    nvgFillColor(
      miau::vg,
      nvgRGBA(
        color.r,
        color.g,
        color.b,
        color.a
      )
    );
    nvgFill(miau::vg);

    float ow = _width - padding.left - padding.right;
    float oh = _height - padding.top - padding.bottom;

    float oy = align_vertical == vertical::center ? (oh - _childrens_height) / 2.0f
             : align_vertical == vertical::bottom ? (oh - _childrens_height)
             : 0.0f;

    nvgScissor(miau::vg, x + padding.left, y + padding.top, ow, oh);

    float offset = y + padding.top + oy;

    for (auto& child : children) {
      float ox = align_horizontal == horizontal::center ? (ow - child->width()) / 2.0f
               : align_horizontal == horizontal::right ? (ow - child->width())
               : 0.0f;
      child->draw(x + padding.left + ox, offset);
      offset += child->height() + spacing;
    }

    nvgResetScissor(miau::vg);
  }

  float column::width() const noexcept { return _width; }
  float column::height() const noexcept { return _height; }

  void stack::update(float x, float y) {
    _width = 0;
    _height = 0;

    for (auto& child : children) {
      child->update(x, y);
      _width = std::max(child->width(), _width);
      _height = std::max(child->height(), _height);
    }

    _childrens_width = _width;
    _childrens_height = _height;

    if (min_width > _width) _width = min_width;
    if (min_height > _height) _height = min_height;

    if (fixed) {
      if (max_width >= 0) _width = max_width;
      if (max_height >= 0) _height = max_height;
    } else {
      _width += padding.left + padding.right;
      _height += padding.top + padding.bottom;
      if (max_width >= 0 && _width > max_width) _width = max_width;
      if (max_height >= 0 && _height > max_height) _height = max_height;
    }
  }

  void stack::draw(float x, float y) {
    nvgBeginPath(miau::vg);
    nvgRoundedRect(miau::vg, x, y, _width, _height, rounded);
    nvgFillColor(miau::vg, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgFill(miau::vg);

    float ow = _width - padding.left - padding.right;
    float oh = _height - padding.top - padding.bottom;

    nvgScissor(miau::vg, x + padding.left, y + padding.top, ow, oh);

    for (auto& child : children) {
      float ox = align_horizontal == horizontal::center ? (ow - child->width()) / 2.0f
               : align_horizontal == horizontal::right ? (ow - child->width())
               : 0.0f;
      float oy = align_vertical == vertical::center ? (oh - child->height()) / 2.0f
               : align_vertical == vertical::bottom ? (oh - child->height())
               : 0.0f;
      child->draw(x + padding.left + ox, y + padding.top + oy);
    }

    nvgResetScissor(miau::vg);
  }

  float stack::width() const noexcept { return _width; }
  float stack::height() const noexcept { return _height; }

  void row::update(float x, float y) {
    _width = 0;
    _height = 0;
    _childrens_width = 0;
    _childrens_height = 0;

    for (auto& child : children) {
      child->update(x + _width, y);
      _width += child->width() + spacing;
      _height = std::max(child->height(), _height);
    }
    _width -= spacing;

    _childrens_width = _width;
    _childrens_height = _height;

    if (min_width > _width) _width = min_width;
    if (min_height > _height) _height = min_height;

    if (fixed) {
      if (max_width >= 0) _width = max_width;
      if (max_height >= 0) _height = max_height;
    } else {
      _width += padding.left + padding.right;
      _height += padding.top + padding.bottom;
      if (max_width >= 0 && _width > max_width) _width = max_width;
      if (max_height >= 0 && _height > max_height) _height = max_height;
    }
  }

  void row::draw(float x, float y) {
    nvgBeginPath(miau::vg);
    nvgRoundedRect(miau::vg, x, y, _width, _height, rounded);
    nvgFillColor(miau::vg, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgFill(miau::vg);

    float ow = _width - padding.left - padding.right;
    float oh = _height - padding.top - padding.bottom;

    float ox = align_horizontal == horizontal::center ? (ow - _childrens_width) / 2.0f
             : align_horizontal == horizontal::right ? (ow - _childrens_width)
             : 0.0f;
    float oy = align_vertical == vertical::center ? (oh - _childrens_height) / 2.0f
             : align_vertical == vertical::bottom ? (oh - _childrens_height)
             : 0.0f;

    nvgScissor(miau::vg, x + padding.left, y + padding.top, ow, oh);

    float offset = x + padding.left + ox;

    for (auto& child : children) {
      child->draw(offset, y + padding.top + oy);
      offset += child->width() + spacing;
    }

    nvgResetScissor(miau::vg);
  }

  float row::width() const noexcept { return _width; }
  float row::height() const noexcept { return _height; }
}
