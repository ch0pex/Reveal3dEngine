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
#include <sstream>

using namespace std::literals;
enum LogLevel : reveal3d::u8 {
    LogError = 0,
    LogWarning,
    LogDebug,
};

#ifdef _DEBUG
inline constexpr LogLevel loglevel = LogDebug;
#else
inline constexpr LogLevel loglevel = LogWarning;
#endif

template<LogLevel lvl> class Logger {
public:
    explicit Logger() {
        if constexpr (lvl == LogError) {
            buffer_.append("[ERROR]: "s);
        }
        else if constexpr (lvl == LogWarning) {
            buffer_.append("[WARNING]: "s);
        }
        else {
            buffer_.append("[DEBUG]: "s);
        }
    }

    Logger& operator<<(auto const& value) {
        fmt::format_to(std::back_inserter(buffer_), "{}", value);
        return *this;
    }

    static std::string log() { return fmt::to_string(persistent_logs_.at(lvl)); }

    ~Logger() {
        buffer_.push_back('\n');
        OutputDebugString(fmt::to_string(buffer_).data());
        persistent_logs_.at(lvl).append(buffer_);

#ifdef _WIN32
#else
        std::cerr << fmt::to_string(buffer_);
#endif
    }

    static void clear() { persistent_logs_.at(lvl).clear(); }

private:
    fmt::memory_buffer buffer_;
    static inline std::array<fmt::memory_buffer, 3> persistent_logs_;
};

// Macro para usar el logger con diferentes niveles
#define logger(level)                                                                                                  \
    if (level > loglevel)                                                                                              \
        ;                                                                                                              \
    else                                                                                                               \
        Logger<level>()
