#ifndef _MIAU_HUD_KEYSTROKE_HH
#define _MIAU_HUD_KEYSTROKE_HH

#include <string_view>

#include "component/ui.hh"
#include "component/box.hh"
#include "component/label.hh"

namespace miau::hud {
  class key : public ui {
  public:
    key(std::string_view text, int code) noexcept;

    void update(float x, float y) override;
    void draw(float x, float y) override;

    float width() const noexcept override;
    float height() const noexcept override;

  private:
    float _counter;
    int _code;

    std::shared_ptr<stack> _back;
    std::shared_ptr<stack> _light;
    std::shared_ptr<label> _key;
  };

  class space : public ui {
  public:
    space() noexcept;

    void update(float x, float y) override;
    void draw(float x, float y) override;

    float width() const noexcept override;
    float height() const noexcept override;

  private:
    float _counter;

    std::shared_ptr<stack> _back;
    std::shared_ptr<stack> _light;
    std::shared_ptr<row> _bar;
  };

  class keystroke : public ui {
  public:
    keystroke() noexcept;

    void update(float x, float y) override;
    void draw(float x, float y) override;

    float width() const noexcept override;
    float height() const noexcept override;

  private:
    std::shared_ptr<column> _column;
  };
}

#endif
