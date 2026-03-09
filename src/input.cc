#include <array>
#include <dlfcn.h>

#include "log.hh"
#include "main.hh"
#include "input.hh"

namespace {
  std::array<std::uint8_t, 1000> states;
}

namespace miau {
  void input_init() noexcept {
    states.fill(2);

    on_key(&states, [](void* states, int code, int action) -> bool {
      MIAU_INFO("{}", code);
      (*static_cast<std::array<std::uint8_t, 1000>*>(states))[code] = action;
      return false;
    });
  }

  bool is_pushed(int code) noexcept { return states[code] == 0; }
  bool is_press(int code) noexcept { return states[code] <= 1; }
  bool is_release(int code) noexcept { return states[code] == 2; }
}
