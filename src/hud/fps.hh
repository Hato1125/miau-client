#ifndef _MIAU_HUD_FPS_HH
#define _MIAU_HUD_FPS_HH

#include "component/ui.hh"
#include "component/box.hh"
#include "component/label.hh"

namespace miau::hud {
  class fps : public ui {
  public:
    fps() noexcept;

    void update(float x, float y) override;
    void draw(float x, float y) override;

    float width() const noexcept override;
    float height() const noexcept override;

  private:
    std::shared_ptr<row> _box;
    std::shared_ptr<label> _icon;
    std::shared_ptr<label> _title;
    std::shared_ptr<label> _fps;

    float _timer;
    int _count;
  };
}

#endif
