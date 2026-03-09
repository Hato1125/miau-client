#ifndef _MIAU_EASING_HH
#define _MIAU_EASING_HH

#include <cmath>

namespace miau::easing {
  inline float linear(float t) noexcept {
    return t;
  }

  inline float in_quad(float t) noexcept {
    return t * t;
  }

  inline float out_quad(float t) noexcept {
    return t * (2.0f - t);
  }

  inline float in_out_quad(float t) noexcept {
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
  }

  inline float in_cubic(float t) noexcept {
    return t * t * t;
  }

  inline float out_cubic(float t) noexcept {
    float u = t - 1.0f;
    return u * u * u + 1.0f;
  }

  inline float in_out_cubic(float t) noexcept {
    return t < 0.5f ? 4.0f * t * t * t : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f;
  }

  inline float smoothstep(float t) noexcept {
    return t * t * (3.0f - 2.0f * t);
  }

  inline float in_elastic(float t) noexcept {
    if (t == 0.0f || t == 1.0f) return t;
    return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * (2.0f * static_cast<float>(M_PI) / 3.0f));
  }

  inline float out_elastic(float t) noexcept {
    if (t == 0.0f || t == 1.0f) return t;
    return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * (2.0f * static_cast<float>(M_PI) / 3.0f)) + 1.0f;
  }

  inline float out_bounce(float t) noexcept {
    if (t < 1.0f / 2.75f) {
      return 7.5625f * t * t;
    } else if (t < 2.0f / 2.75f) {
      t -= 1.5f / 2.75f;
      return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5f / 2.75f) {
      t -= 2.25f / 2.75f;
      return 7.5625f * t * t + 0.9375f;
    } else {
      t -= 2.625f / 2.75f;
      return 7.5625f * t * t + 0.984375f;
    }
  }

  inline float exp_lerp(float t) noexcept {
    return 1.0f - std::exp(-t);
  }
}

#endif
