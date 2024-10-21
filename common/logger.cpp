/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file logger.cpp
 * @version 1.0
 * @date 02/06/2024
 * @brief Logger 
 *
 * Logger class 
 */

#include "logger.hpp"

std::stringstream Logger::persistent_logs_[3];

Logger::Logger(LogLevel log_level) : level_(log_level) {

    switch (log_level) {
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

std::string Logger::log(LogLevel log_level) {
    return persistent_logs_[log_level].str();
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

void Logger::clear(LogLevel log_level) {
    Logger::persistent_logs_[log_level].str("");
}
