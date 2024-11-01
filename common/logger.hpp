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
#include <iostream>
#include <sstream>

enum LogLevel : reveal3d::u8 {
    LogError = 0,
    LogWarning,
    LogDebug,
};

inline constexpr LogLevel loglevel = LogDebug;

class Logger {
public:
    Logger(LogLevel log_level) : level_(log_level) {

        switch (log_level) {
            case LogError:
                buffer_ << "ERROR MSG" << ": ";
                break;
            case LogWarning:
                buffer_ << "WARNING MSG" << ": ";
                break;
            case LogDebug:
                buffer_ << "DEBUG MSG" << ": ";
                break;
        }
    }

    template<typename T>
    Logger& operator<<(T const& value) {
        buffer_ << value;
        return *this;
    }

    static std::string log(LogLevel log_level) { return Logger::persistent_logs_.at(log_level).str(); }

    ~Logger() {
        buffer_ << '\n';
        Logger::persistent_logs_.at(level_) << buffer_.str();
#ifdef _WIN32
        OutputDebugStringA(buffer_.str().c_str());
#else
        std::cerr << buffer_.str();
#endif
    }

    static void clear(LogLevel log_level) { Logger::persistent_logs_.at(log_level).str(""); }

private:
    LogLevel level_;
    std::ostringstream buffer_;
    static inline std::array<std::stringstream, 3> persistent_logs_;
};


#define logger(level)                                                                                                  \
    if (level > loglevel)                                                                                              \
        ;                                                                                                              \
    else                                                                                                               \
        Logger(level)
