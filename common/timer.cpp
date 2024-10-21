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
    second_per_count_(0.0), delta_time_(-1.0), base_time_(0), paused_time_(0), stop_time_(0), prev_time_(0),
    curr_time_(0), prev_total_frames_(0), total_time_(0), total_frames_(0), stopped_(false)
{
    queryFrequency(counts_per_second_);
    second_per_count_ = 1.0 / (f64)counts_per_second_;

    input::add_handler_up(input::Action::ScenePause,
                          {[this](input::Action act, input::type type) { pause(act, type); }});
}


void Timer::reset() {
    i64 curr_time = 0;
    queryCounter(curr_time);

    base_time_ = curr_time;
    prev_time_ = curr_time;
    stop_time_ = 0;
    stopped_ = false;
}

void Timer::start() {
   if (stopped_) {
       i64 start_time = 0;
       queryCounter(start_time);

       paused_time_ += (start_time - stop_time_);
       prev_time_ = start_time;
       stopped_ = false;
       stop_time_ = 0;
   }
}

void Timer::stop() {
    if (!stopped_) {
        i64 curr_time = 0;
        queryCounter(curr_time);
        stop_time_ = curr_time;
        stopped_ = true;
    }
}

void Timer::tick() {

    i64 curr_time = 0;
    queryCounter(curr_time);
    total_frames_++;

    curr_time_ = curr_time;
    delta_time_ = (curr_time - prev_time_) * second_per_count_;
    frame_time_ = delta_time_;
    prev_time_ = curr_time_;

    const u64 newTotalSeconds =static_cast<u64>(floor((curr_time - base_time_) * second_per_count_));

    if (total_time_ < newTotalSeconds) {
        total_time_ = newTotalSeconds;
        fps_ = total_frames_ - prev_total_frames_;
        prev_total_frames_ = total_frames_;
    }

    if (delta_time_ < 0.0) {
        delta_time_ = 0.0;
    }

    if (stopped_) {
        delta_time_ = 0.0;
    }
}
f32 Timer::totalTime() const {
    if (stopped_) {
        return ((stop_time_ - paused_time_) - base_time_) * second_per_count_;
    }
    i64 curr_time = 0;

    queryCounter(curr_time);
    return ((curr_time - paused_time_) - base_time_) * second_per_count_;
}

f32 Timer::diff(f32 time) const {
    return totalTime() - time;
}

void Timer::pause(input::Action act, input::type type) {
    if(stopped_) {
        start();
    } else {
        stop();
    }
}

}