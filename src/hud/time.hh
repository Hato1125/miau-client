#ifndef _MIAU_HUD_TIME_HH
#define _MIAU_HUD_TIME_HH

#include "component/ui.hh"
#include "component/box.hh"
#include "component/label.hh"

namespace miau::hud {
  class time : public ui {
  public:
    time() noexcept;

    void update(float x, float y) override;
    void draw(float x, float y) override;

    float width() const noexcept override;
    float height() const noexcept override;

  private:
    std::shared_ptr<row> _box;
    std::shared_ptr<label> _icon;
    std::shared_ptr<label> _ampm;
    std::shared_ptr<label> _time;
  };
}

#endif
