#include <chrono>
#include <ctime>
#include <format>

#include "time.hh"
#include "main.hh"

namespace miau::hud {
  time::time() noexcept {
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
    _icon->text = "\ue8b5";
    _box->children.push_back(_icon);

    _ampm = std::make_shared<label>();
    _ampm->font = text_font();
    _ampm->offset = 3;
    _ampm->size = 22.5;
    _ampm->color = colors::white;
    _ampm->text = "AM";
    _box->children.push_back(_ampm);

    _time = std::make_shared<label>();
    _time->font = text_font();
    _time->offset = 3;
    _time->size = 22.5;
    _time->color = colors::white;
    _time->text = "00:00";
    _box->children.push_back(_time);
  }

  void time::update(float x, float y) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm local{};
    localtime_r(&now, &local);

    int h = local.tm_hour;
    int m = local.tm_min;

    _ampm->text = h < 12 ? "AM" : "PM";
    int h12 = h % 12;
    if (h12 == 0) h12 = 12;
    _time->text = std::format("{:02}:{:02}", h12, m);

    _box->update(x, y);
  }

  void time::draw(float x, float y) {
    _box->draw(x, y);
  }

  float time::width() const noexcept { return _box->width(); }
  float time::height() const noexcept { return _box->height(); }
}
