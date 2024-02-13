/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file logger.hpp
 * @version 1.0
 * @date 08/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "platform.hpp"

#include <iostream>
#include <sstream>


enum LogLevel {
    logERROR = 0,
    logDEBUG,
};

class Logger
{
public:
    Logger(LogLevel _loglevel = logERROR) {
        buffer_ << _loglevel << " :"
                << std::string( _loglevel > logDEBUG ? (_loglevel - logDEBUG) * 4 : 1 , ' ');
    }

    template <typename T>
    Logger & operator<<(T const & value)
    {
        buffer_ << value;
        return *this;
    }

    ~Logger()
    {
        buffer_ << std::endl;
#ifdef _WIN32
        OutputDebugStringA(buffer_.str().c_str());
#else
        std::cerr << buffer_.str();
#endif
    }

private:
    std::ostringstream buffer_;
};

extern LogLevel loglevel;

#define log(level) \
if (level > loglevel) ; \
else Logger(level)

