/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file logger.hpp
 * @version 1.0
 * @date 08/03/2024
 * @brief Logger
 *
 * Logger class
 */

#pragma once

#include "platform.hpp"
#include "primitive_types.hpp"

#include <array>
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>

using namespace std::literals;

enum LogLevel : reveal3d::u8 { LogError = 0, LogWarning, LogInfo, LogAll };

inline constexpr LogLevel loglevel = LogInfo;

template<LogLevel lvl>
class Logger {
public:
  explicit Logger() {
    if constexpr (lvl == LogError) {
      buffer_.append("[ERROR]: "s);
    }
    else if constexpr (lvl == LogWarning) {
      buffer_.append("[WARNING]: "s);
    }
    else if constexpr (lvl == LogInfo) {
      buffer_.append("[INFO]: "s);
    }
  }

  Logger& operator<<(auto const& value) {
    fmt::format_to(std::back_inserter(buffer_), "{}", value);
    return *this;
  }

  static void append(auto const& range) { persistent_log_.append(range); }

  static std::string log() { return fmt::to_string(persistent_log_); }

  ~Logger() {
    buffer_.push_back('\n');
    persistent_log_.append(buffer_);
    if constexpr (lvl != LogAll) {
      Logger<LogAll>::append(buffer_);
    }

#ifdef _WIN32
    OutputDebugString(fmt::to_string(buffer_).data());
#else
    std::cerr << fmt::to_string(buffer_);
#endif
  }

  static void clear() { persistent_log_.clear(); }

private:
  fmt::memory_buffer buffer_;
  static inline fmt::memory_buffer persistent_log_;
};

// Macro para usar el logger con diferentes niveles
#define logger(level)                                                                                                  \
  if (level > loglevel)                                                                                                \
    ;                                                                                                                  \
  else                                                                                                                 \
    Logger<level>()
