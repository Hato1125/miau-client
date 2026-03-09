#ifndef _MIAU_LOG_HH
#define _MIAU_LOG_HH

#include <print>
#include <format>
#include <string_view>
#include <source_location>

namespace detail {
  template <typename ...Args>
  void log(
    std::string_view esc,
    std::string_view category,
    std::source_location location,
    std::format_string<Args...> fmt,
    Args&&... args
  ) noexcept {
    std::println(
      "{}[miau::{} {}:{}] {}\033[0m",
      esc,
      category,
      location.line(),
      location.column(),
      std::format(fmt, std::forward<Args>(args)...)
    );
  }
}

#ifdef RELEASE
  #define MIAU_INFO(fmt, ...)
  #define MIAU_TRACE(fmt, ...)
#else
  #define MIAU_INFO(fmt, ...) detail::log("\x1b[32m", "info", std::source_location::current(), fmt __VA_OPT__(, ) __VA_ARGS__)
  #define MIAU_TRACE(fmt, ...) detail::log("\x1b[34m", "trace", std::source_location::current(), fmt __VA_OPT__(, ) __VA_ARGS__)
#endif

#define MIAU_WARN(fmt, ...) detail::log("\x1b[33m", "warn", std::source_location::current(), fmt __VA_OPT__(, ) __VA_ARGS__)
#define MIAU_ERROR(fmt, ...) detail::log("\x1b[31m", "error", std::source_location::current(), fmt __VA_OPT__(, ) __VA_ARGS__)
#define MIAU_FATAL(fmt, ...) detail::log("\x1b[31m", "fatal", std::source_location::current(), fmt __VA_OPT__(, ) __VA_ARGS__)

#endif
