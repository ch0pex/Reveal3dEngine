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

#include <iostream>
#include <sstream>

namespace reveal3d {

}

enum LogLevel {
    logERROR = 0,
    logWARNING,
    logDEBUG,
};

class Logger
{
public:
    Logger(LogLevel log_level = logERROR);

    template <typename T>
    Logger & operator<<(T const & value)
    {
        buffer_ << value;
        return *this;
    }

    static void clear(LogLevel log_level);
    static std::string log(LogLevel log_level);
    ~Logger();

private:
    LogLevel level_;
    std::ostringstream buffer_;
    static std::stringstream persistent_logs_[3];
};

extern LogLevel loglevel;

#define logger(level) \
if (level > loglevel) ; \
else Logger(level)

