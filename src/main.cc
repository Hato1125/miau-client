#include <chrono>

#include <dlfcn.h>
#include <jni.h>
#include <android/input.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#define NANOVG_GLES2_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

#include "main.hh"
#include "log.hh"
#include "input.hh"

#include "hud/fps.hh"
#include "hud/time.hh"
#include "hud/keystroke.hh"

namespace {
  std::shared_ptr<miau::hud::fps> fps;
  std::shared_ptr<miau::hud::time> time;
  std::shared_ptr<miau::hud::keystroke> key;

  GameWindowHandle* game_window_handle;
  GameWindowHandle* (*game_window_get_primary_window)();
  void (*game_window_add_keyboard_callback)(GameWindowHandle*, void*, bool (*)(void*, int, int));
  void (*game_window_add_mouse_button_callback)(GameWindowHandle*, void*, bool (*)(void*, double, double, int, int));
  void (*game_window_add_mouse_position_callback)(GameWindowHandle*, void*, bool (*)(void*, double, double, bool));
  void (*game_window_add_mouse_scroll_callback)(GameWindowHandle*, void*, bool (*)(void*, double, double, double, double));

  void (*mcpelauncher_preinithook)(
    const char* sym,
    void* val,
    void** orig
  );


  decltype(&eglSwapBuffers) egl_swap_buffers_orig;
  decltype(&eglMakeCurrent) egl_make_current_orig;
  void* swappy_swap_placeholder;

  std::chrono::duration<float> duration;
  int text_font = -1;
  int icon_font = -1;

  void loop(EGLDisplay dpy, EGLSurface surface) noexcept {
    if (!miau::vg) {
      return;
    }

    static auto last_time = std::chrono::steady_clock::now();

    EGLint width = 0;
    EGLint height = 0;
    eglQuerySurface(dpy, surface, EGL_WIDTH, &width);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &height);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, width, height);

    nvgBeginFrame(miau::vg, width, height, 1.f);

    fps->update(20, 50 + 100);
    fps->draw(20, 50 + 100);

    time->update(20, 120 + 100);
    time->draw(20, 120 + 100);

    key->update(20, 300);
    key->draw(20, 300);

    nvgEndFrame(miau::vg);

    glViewport(
      viewport[0],
      viewport[1],
      viewport[2],
      viewport[3]
    );

    duration = std::chrono::steady_clock::now() - last_time;
    last_time = std::chrono::steady_clock::now();
  }

  EGLBoolean swappy_swap(EGLDisplay dpy, EGLSurface surface) noexcept {
    loop(dpy, surface);
    return egl_swap_buffers_orig(dpy, surface);
  }

  EGLBoolean egl_swap_buffers(EGLDisplay dpy, EGLSurface surface) noexcept {
    loop(dpy, surface);
    return egl_swap_buffers_orig(dpy, surface);
  }

  EGLBoolean egl_make_current(
    EGLDisplay dpy,
    EGLSurface draw,
    EGLSurface read,
    EGLContext ctx
  ) noexcept {
    EGLBoolean result = egl_make_current_orig(dpy, draw, read, ctx);

    if (!miau::vg && draw) {
      miau::vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

      if (!miau::vg) {
        MIAU_FATAL("nvgCreateGLES2 failed");
      } else {
        MIAU_TRACE("nvgCreateGLES2 success");
      }

      text_font = nvgCreateFont(miau::vg, "text-font", "/usr/share/fonts/260106Y2P5QRZLKY/TX-02-02X193PL/BerkeleyMonoTrial-Regular.ttf");
      icon_font = nvgCreateFont(miau::vg, "icon-font", "/usr/share/fonts/ttf-material-symbols-variable/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf");

      miau::input_init();

      fps = std::make_shared<miau::hud::fps>();
      time = std::make_shared<miau::hud::time>();
      key = std::make_shared<miau::hud::keystroke>();
    }

    return result;
  }

}

namespace miau {
  int text_font() noexcept { return ::text_font; }
  int icon_font() noexcept { return ::icon_font; }

  float frame_time_ms() noexcept { return duration.count() * 1000.f; }
  float frame_time_s() noexcept { return duration.count(); }

  static GameWindowHandle* get_handle() noexcept {
    if (!game_window_handle && game_window_get_primary_window)
      game_window_handle = game_window_get_primary_window();
    return game_window_handle;
  }

  void on_key(void* user, bool (*callback)(void* user, int keyCode, int action)) noexcept {
    auto h = get_handle();
    if (game_window_add_keyboard_callback && h)
      game_window_add_keyboard_callback(h, user, callback);
  }

  void on_mouse_button(void* user, bool (*callback)(void* user, double x, double y, int button, int action)) noexcept {
    auto h = get_handle();
    if (game_window_add_mouse_button_callback && h)
      game_window_add_mouse_button_callback(h, user, callback);
  }

  void on_mouse_position(void* user, bool (*callback)(void* user, double x, double y, bool relative)) noexcept {
    auto h = get_handle();
    if (game_window_add_mouse_position_callback && h)
      game_window_add_mouse_position_callback(h, user, callback);
  }

  void on_mouse_scroll(void* user, bool (*callback)(void* user, double x, double y, double dx, double dy)) noexcept {
    auto h = get_handle();
    if (game_window_add_mouse_scroll_callback && h)
      game_window_add_mouse_scroll_callback(h, user, callback);
  }
}

extern "C" [[gnu::visibility("default")]] void mod_preinit() noexcept {
  auto dl = dlopen("libmcpelauncher_mod.so", RTLD_NOW);

  if (dl) {
    mcpelauncher_preinithook = reinterpret_cast<decltype(mcpelauncher_preinithook)>(
      dlsym(dl, "mcpelauncher_preinithook")
    );

    if (mcpelauncher_preinithook) {
      mcpelauncher_preinithook(
        "SwappyGL_swap",
        reinterpret_cast<void*>(&swappy_swap),
        &swappy_swap_placeholder
      );

      mcpelauncher_preinithook(
        "eglSwapBuffers",
        reinterpret_cast<void*>(&egl_swap_buffers),
        reinterpret_cast<void**>(&egl_swap_buffers_orig)
      );

      mcpelauncher_preinithook(
        "eglMakeCurrent",
        reinterpret_cast<void*>(&egl_make_current),
        reinterpret_cast<void**>(&egl_make_current_orig)
      );
    }

    dlclose(dl);
    MIAU_TRACE("preinit done");
  } else {
    MIAU_FATAL("libmcpelauncher_mod.so not found");
  }
}

extern "C" [[gnu::visibility("default")]] void mod_init() noexcept {
  auto gw = dlopen("libmcpelauncher_gamewindow.so", RTLD_NOW);

  if (gw) {
    game_window_get_primary_window = reinterpret_cast<decltype(game_window_get_primary_window)>(
      dlsym(gw, "game_window_get_primary_window")
    );

    game_window_add_keyboard_callback = reinterpret_cast<decltype(game_window_add_keyboard_callback)>(
      dlsym(gw, "game_window_add_keyboard_callback")
    );
    game_window_add_mouse_button_callback = reinterpret_cast<decltype(game_window_add_mouse_button_callback)>(
      dlsym(gw, "game_window_add_mouse_button_callback")
    );
    game_window_add_mouse_position_callback = reinterpret_cast<decltype(game_window_add_mouse_position_callback)>(
      dlsym(gw, "game_window_add_mouse_position_callback")
    );
    game_window_add_mouse_scroll_callback = reinterpret_cast<decltype(game_window_add_mouse_scroll_callback)>(
      dlsym(gw, "game_window_add_mouse_scroll_callback")
    );

    dlclose(gw);
    MIAU_TRACE("gamewindow init done");
  } else {
    MIAU_FATAL("libmcpelauncher_gamewindow.so not found");
  }

  auto egl = dlopen("libEGL.so", RTLD_NOW);

  if (egl) {
    egl_swap_buffers_orig = reinterpret_cast<decltype(egl_swap_buffers_orig)>(
      dlsym(egl, "eglSwapBuffers")
    );
    if (!egl_swap_buffers_orig) MIAU_WARN("eglSwapBuffers not found");

    egl_make_current_orig = reinterpret_cast<decltype(egl_make_current_orig)>(
      dlsym(egl, "eglMakeCurrent")
    );
    if (!egl_make_current_orig) MIAU_FATAL("eglMakeCurrent not found");

    dlclose(egl);
    MIAU_TRACE("mod_init done");
  } else {
    MIAU_FATAL("libEGL.so not found");
  }
}
