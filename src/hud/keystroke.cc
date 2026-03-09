#include "input.hh"
#include "easing.hh"
#include "main.hh"
#include "keystroke.hh"

namespace miau::hud {
  key::key(std::string_view text, int code) noexcept : _code(code) {
    _counter = 0.0f;

    _back = std::make_shared<stack>();
    _back->align_vertical = vertical::center;
    _back->align_horizontal = horizontal::center;
    _back->fixed = true;
    _back->max_width = 100;
    _back->max_height = 100;
    _back->rounded = 35;
    _back->color = colors::black;

    _light = std::make_shared<stack>();
    _light->fixed = true;
    _light->max_width = 0;
    _light->max_height = 0;
    _light->rounded = 35;
    _light->color = colors::white;
    _back->children.push_back(_light);

    _key = std::make_shared<label>();
    _key->font = text_font();
    _key->size = 35;
    _key->text = text;
    _key->color = colors::white;
    _back->children.push_back(_key);
  }

  void key::update(float x, float y) {
    _counter += (is_press(_code) - _counter) * easing::exp_lerp(40.0f * frame_time_s());

    _light->max_width = _back->width() * _counter;
    _light->max_height = _back->height() * _counter;

    _back->update(x, y);
    _key->color.r = 255 - (255 * _counter);
    _key->color.g = 255 - (255 * _counter);
    _key->color.b = 255 - (255 * _counter);
  }

  void key::draw(float x, float y) {
    _back->draw(x, y);
  }

  float key::width() const noexcept {
    return _back->width();
  }

  float key::height() const noexcept {
    return _back->height();
  }

  space::space() noexcept {
    _counter = 0.0f;

    _back = std::make_shared<stack>();
    _back->align_vertical = vertical::center;
    _back->align_horizontal = horizontal::center;
    _back->fixed = true;
    _back->max_width = 316;
    _back->max_height = 80;
    _back->rounded = 35;
    _back->color = colors::black;

    _light = std::make_shared<stack>();
    _light->fixed = true;
    _light->max_width = 0;
    _light->max_height = 0;
    _light->rounded = 35;
    _light->color = colors::white;
    _back->children.push_back(_light);

    _bar = std::make_shared<row>();
    _bar->max_width = 200;
    _bar->max_height = 4;
    _bar->fixed = true;
    _bar->rounded = 1.5f;
    _back->children.push_back(_bar);
  }

  void space::update(float x, float y) {
    _counter += (is_press(32) - _counter) * easing::exp_lerp(40.0f * frame_time_s());

    _light->max_width = _back->width() * _counter;
    _light->max_height = _back->height() * _counter;

    _back->update(x, y);
    _bar->color.r = 255 - (255 * _counter);
    _bar->color.g = 255 - (255 * _counter);
    _bar->color.b = 255 - (255 * _counter);
  }

  void space::draw(float x, float y) {
    _back->draw(x, y);
  }

  float space::width() const noexcept {
    return _back->width();
  }

  float space::height() const noexcept {
    return _back->height();
  }

  keystroke::keystroke() noexcept {
    _column = std::make_shared<column>();
    _column->align_vertical = vertical::center;
    _column->align_horizontal = horizontal::center;
    _column->spacing = 8;
    _column->color.a = 0;

    auto row1 = std::make_shared<row>();
    row1->align_vertical = vertical::center;
    row1->align_horizontal = horizontal::center;
    row1->spacing = 8;
    row1->color.a = 0;
    row1->children.push_back(std::make_shared<key>("W", 87));
    _column->children.push_back(row1);

    auto row2 = std::make_shared<row>();
    row2->align_vertical = vertical::center;
    row2->align_horizontal = horizontal::center;
    row2->spacing = 8;
    row2->color.a = 0;
    row2->children.push_back(std::make_shared<key>("A", 65));
    row2->children.push_back(std::make_shared<key>("S", 83));
    row2->children.push_back(std::make_shared<key>("D", 68));
    _column->children.push_back(row2);

    auto row3 = std::make_shared<row>();
    row3->align_vertical = vertical::center;
    row3->align_horizontal = horizontal::center;
    row3->spacing = 8;
    row3->color.a = 0;
    row3->children.push_back(std::make_shared<space>());
    _column->children.push_back(row3);
  }

  void keystroke::update(float x, float y) { _column->update(x, y); }
  void keystroke::draw(float x, float y) { _column->draw(x, y); }

  float keystroke::width() const noexcept {  return _column->width(); }
  float keystroke::height() const noexcept {  return _column->height(); }
}
