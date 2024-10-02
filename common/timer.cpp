/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file timer.cpp
 * @version 1.0
 * @date 06/03/2024
 * @brief Timing utilities 
 *
 * Timer class definitions
 */

#include "timer.hpp"

namespace reveal3d {

Timer::Timer() :
    secondPerCount_(0.0), deltaTime_(-1.0), baseTime_(0), pausedTime_(0), stopTime_(0), prevTime_(0), currTime_(0),
    prevTotalFrames_(0), totalTime_(0), totalFrames_(0), stopped_(false)
{
    QueryFrequency(countsPerSecond_);
    secondPerCount_ = 1.0 / (f64)countsPerSecond_;

    inputSystem_.AddHandlerUp(input::action::scene_pause, {[this](input::action act, input::type type){ Pause(act, type); }});
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

    i64 currTime = 0;
    QueryCounter(currTime);
    totalFrames_++;

    currTime_ = currTime;
    deltaTime_ = (currTime - prevTime_) * secondPerCount_;
    frameTime_ = deltaTime_;
    prevTime_ = currTime_;

    const u64 newTotalSeconds =static_cast<u64>(floor((currTime - baseTime_) * secondPerCount_));

    if (totalTime_ < newTotalSeconds) {
        totalTime_ = newTotalSeconds;
        fps_ = totalFrames_ - prevTotalFrames_;
        prevTotalFrames_ = totalFrames_;
    }

    if (deltaTime_ < 0.0) {
        deltaTime_ = 0.0;
    }

    if (stopped_) {
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

f32 Timer::Diff(f32 time) const {
    return TotalTime() - time;
}

void Timer::Pause(input::action act, input::type type) {
    if(stopped_) {
        Start();
    } else {
       Stop();
    }
}

}