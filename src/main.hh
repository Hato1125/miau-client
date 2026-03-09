#ifndef MOD_TEST_MAIN_HH
#define MOD_TEST_MAIN_HH

#include "nanovg.h"

struct GameWindowHandle;

namespace miau {
  inline NVGcontext* vg;

  int text_font() noexcept;
  int icon_font() noexcept;

  float frame_time_ms() noexcept;
  float frame_time_s() noexcept;

  void on_key(void* user, bool (*callback)(void* user, int keyCode, int action)) noexcept;
  void on_mouse_button(void* user, bool (*callback)(void* user, double x, double y, int button, int action)) noexcept;
  void on_mouse_position(void* user, bool (*callback)(void* user, double x, double y, bool relative)) noexcept;
  void on_mouse_scroll(void* user, bool (*callback)(void* user, double x, double y, double dx, double dy)) noexcept;
}

#endif
