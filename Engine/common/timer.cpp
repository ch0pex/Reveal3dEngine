/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file timer.cpp
 * @version 1.0
 * @date 06/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "timer.hpp"

namespace reveal3d {

Timer::Timer() :
    secondPerCount_(0.0), deltaTime_(-1.0), baseTime_(0), pausedTime_(0), stopTime_(0), prevTime_(0), currTime_(0),
    stopped_(false)
{
    i64 countsPerSec = 0;
    QueryFrequency(countsPerSec);
    secondPerCount_ = 1.0 / (f64)countsPerSec;
}


void Timer::Reset() {
    i64 currTime = 0;
    QueryCounter(currTime);

    baseTime_ = currTime;
    prevTime_ = currTime;
    stopTime_ = 0;
    stopped_ = false;
}

void Timer::Start() {
   if (stopped_) {
       i64 startTime = 0;
       QueryCounter(startTime);

       pausedTime_ += (startTime - stopTime_);
       prevTime_ = startTime;
       stopped_ = false;
       stopTime_ = 0;
   }
}

void Timer::Stop() {
    if (!stopped_) {
        i64 currTime = 0;
        QueryCounter(currTime);
        stopTime_ = currTime;
        stopped_ = true;
    }
}

void Timer::Tick() {
    if (stopped_) {
        deltaTime_ = 0.0;
        return;
    }

    i64 currTime = 0;
    QueryCounter(currTime);

    currTime_ = currTime;
    deltaTime_ = (currTime - prevTime_) * secondPerCount_;
    prevTime_ = currTime_;

    if (deltaTime_ < 0.0) {
        deltaTime_ = 0.0;
    }
}
f32 Timer::TotalTime() const {
    if (stopped_) {
        return ((stopTime_ - pausedTime_) - baseTime_) * secondPerCount_;
    }
    i64 currTime = 0;

    QueryCounter(currTime);
    return ((currTime - pausedTime_) - baseTime_) * secondPerCount_;
}

}