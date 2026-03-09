#ifndef MOD_TEST_MAIN_HH
#define MOD_TEST_MAIN_HH

#include "nanovg.h"

namespace miau {
  inline NVGcontext* vg;

  int text_font() noexcept;
  int icon_font() noexcept;

  float frame_time_ms() noexcept;
  float frame_time_s() noexcept;
}

#endif
