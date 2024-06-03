/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file logger.cpp
 * @version 1.0
 * @date 02/06/2024
 * @brief Short description
 *
 * Longer description
 */

#include "logger.hpp"

std::stringstream Logger::persistent_logs_[3];

Logger::Logger(LogLevel logLevel) : level_(logLevel) {

    switch (logLevel) {
        case logERROR:
            buffer_ << "ERROR MSG" << ": ";
            break;
        case logWARNING:
            buffer_ << "WARNING MSG" << ": ";
            break;
        case logDEBUG:
            buffer_ << "DEBUG MSG" << ": ";
            break;
    }
}

std::string Logger::Log(LogLevel logLevel) {
    return persistent_logs_[logLevel].str();
}

Logger::~Logger()
{
    buffer_ << std::endl;
    persistent_logs_[level_] << buffer_.str();
#ifdef _WIN32
    OutputDebugStringA(buffer_.str().c_str());
#else
    std::cerr << buffer_.str();
#endif
}

void Logger::Clear(LogLevel logLevel) {
    Logger::persistent_logs_[logLevel].str("");
}
