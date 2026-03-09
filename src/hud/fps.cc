#include <format>

#include "fps.hh"
#include "main.hh"

namespace miau::hud {
  fps::fps() noexcept {
    _box = std::make_shared<row>();
    _box->padding = {
      .left = 29,
      .right = 29,
    };
    _box->align_vertical = vertical::center;
    _box->align_horizontal = horizontal::center;
    _box->spacing = 13.5;
    _box->rounded = 28.5;
    _box->min_height = 60;
    _box->color = colors::black;


    _icon = std::make_shared<label>();
    _icon->font = icon_font();
    _icon->offset = 1;
    _icon->size = 28;
    _icon->color = colors::white;
    _icon->text = "\ueb77";
    _box->children.push_back(_icon);

    _title = std::make_shared<label>();
    _title->font = text_font();
    _title->offset = 3;
    _title->size = 22.5;
    _title->color = colors::white;
    _title->text = "FPS";
    _box->children.push_back(_title);

    _fps = std::make_shared<label>();
    _fps->font = text_font();
    _fps->offset = 3;
    _fps->size = 22.5;
    _fps->color = colors::white;
    _fps->text = "0";
    _box->children.push_back(_fps);

    _timer = 0.0f;
    _count = 0;
  }

  void fps::update(float x, float y) {
    _timer += frame_time_s();
    _count++;

    if (_timer >= 1.0f) {
      _fps->text = std::format("{}", _count);
      _timer = 0.0f;
      _count = 0;
    }

    _box->update(x, y);
  }

  void fps::draw(float x, float y) {
    _box->draw(x, y);
  }

  float fps::width() const noexcept { return _box->width(); }
  float fps::height() const noexcept { return _box->width(); }
}
