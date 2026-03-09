#include <dlfcn.h>
#include <jni.h>

#include <chrono>

#include <EGL/egl.h>
#include <GLES2/gl2.h>


#define NANOVG_GLES2_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

#include "main.hh"
#include "log.hh"

static void (*mcpelauncher_preinithook)(
  const char* sym,
  void* val,
  void** orig
);

static decltype(&eglSwapBuffers) egl_swap_buffers_orig;
static decltype(&eglMakeCurrent) egl_make_current_orig;
static void* swappy_swap_placeholder;

int font = -1;

static void loop(EGLDisplay dpy, EGLSurface surface) noexcept {
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
  nvgEndFrame(miau::vg);

  glViewport(
    viewport[0],
    viewport[1],
    viewport[2],
    viewport[3]
  );

  last_time = std::chrono::steady_clock::now();
}

static EGLBoolean swappy_swap(
  EGLDisplay dpy,
  EGLSurface surface
) noexcept {
  loop(dpy, surface);
  return egl_swap_buffers_orig(dpy, surface);
}

static EGLBoolean egl_swap_buffers(
  EGLDisplay dpy,
  EGLSurface surface
) noexcept {
  loop(dpy, surface);
  return egl_swap_buffers_orig(dpy, surface);
}

static EGLBoolean egl_make_current(
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
  }

  return result;
}

extern "C" __attribute__ ((visibility ("default"))) void mod_preinit() noexcept {
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

extern "C" __attribute__ ((visibility ("default"))) void mod_init() noexcept {
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
