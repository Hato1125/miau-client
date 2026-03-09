#ifndef _MIAU_INPUT_HH
#define _MIAU_INPUT_HH

namespace miau {
  void input_init() noexcept;
  void input_update() noexcept;

  bool is_pushed(int keyCode) noexcept;
  bool is_press(int keyCode) noexcept;
  bool is_release(int keyCode) noexcept;
}

#endif
