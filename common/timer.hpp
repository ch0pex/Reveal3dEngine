/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file timer.hpp
 * @version 1.0
 * @date 06/03/2024
 * @brief Timer class header file
 *
 * This file declares the timer class used in the library
 * to deal with time, deltatime, etc...
 *
 */

#pragma once

#include <algorithm>
#include <chrono>

#include "input/input.hpp"
#include "platform.hpp"
#include "primitive_types.hpp"


namespace reveal3d {

class Timer {
public:
  Timer() {
    queryFrequency(counts_per_second_);
    second_per_count_ = 1.0 / static_cast<f64>(counts_per_second_);

    add_handler_up(input::Action::ScenePause, {[this](input::Action act, input::type type) { pause(act, type); }});
  }

  [[nodiscard]] f64 deltaTime() const { return delta_time_; }

  [[nodiscard]] f64 frameTime() const { return frame_time_; }

  [[nodiscard]] f64 fps() const { return 1 / delta_time_; }

  [[nodiscard]] f64 meanFps() const { return static_cast<f64>(total_frames_) / totalTime(); }

  [[nodiscard]] u64 averageFps() const { return fps_; }

  [[nodiscard]] u64 totalFrames() const { return total_frames_; }

  [[nodiscard]] bool isRunning() const { return !stopped_; };

  [[nodiscard]] f64 diff(const f64 time) const { return totalTime() - time; }

  [[nodiscard]] f64 totalTime() const {
    if (stopped_)
      return ((stop_time_ - paused_time_) - base_time_) * second_per_count_;
    i64 curr_time = 0;

    queryCounter(curr_time);
    return ((curr_time - paused_time_) - base_time_) * second_per_count_;
  }


  void reset() {
    i64 curr_time = 0;
    queryCounter(curr_time);

    base_time_ = curr_time;
    prev_time_ = curr_time;
    stop_time_ = 0;
    stopped_ = false;
  }

  void start() {
    if (stopped_) {
      i64 start_time = 0;
      queryCounter(start_time);

      paused_time_ += (start_time - stop_time_);
      prev_time_ = start_time;
      stopped_ = false;
      stop_time_ = 0;
    }
  }

  void stop() {
    if (!stopped_) {
      i64 curr_time = 0;
      queryCounter(curr_time);
      stop_time_ = curr_time;
      stopped_ = true;
    }
  }

  void tick() {
    i64 curr_time = 0;
    queryCounter(curr_time);
    total_frames_++;

    curr_time_ = curr_time;
    delta_time_ = (curr_time - prev_time_) * second_per_count_;
    frame_time_ = delta_time_;
    prev_time_ = curr_time_;

    if (const u64 new_total_seconds = static_cast<u64>(floor((curr_time - base_time_) * second_per_count_));
        total_time_ < new_total_seconds) {
      total_time_ = new_total_seconds;
      fps_ = total_frames_ - prev_total_frames_;
      prev_total_frames_ = total_frames_;
    }

    delta_time_ = std::max(delta_time_, 0.0);

    [[unlikely]] if (stopped_) { delta_time_ = 0.0; }
  }

  void pause(input::Action act, input::type type) {
    if (stopped_)
      start();
    else
      stop();
  }

private:
#ifdef _WIN32
  static void queryFrequency(i64& time) { QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&time)); }
  static void queryCounter(i64& time) { QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time)); }
#else
  static inline void QueryFrequency(i64& time) { time = 1000000000; }
  static inline void QueryCounter(i64& time) {
    auto now = std::chrono::high_resolution_clock::now();
    auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
    time = now_ns.time_since_epoch().count();
  }
#endif

  f64 second_per_count_ {};
  i64 counts_per_second_ {};
  f64 delta_time_ {}; // Affected by pause
  f64 frame_time_ {}; // Non afected by pause

  i64 base_time_ {};
  i64 paused_time_ {};
  i64 stop_time_ {};
  i64 prev_time_ {};
  i64 curr_time_ {};

  u64 total_time_ {};
  u16 fps_ {};
  u64 total_frames_ {};
  u64 prev_total_frames_ {};
  bool stopped_ {false};
};


} // namespace reveal3d
