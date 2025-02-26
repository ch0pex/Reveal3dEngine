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

#include "input/input.hpp"
#include "platform.hpp"
#include "primitive_types.hpp"

#include <algorithm>
#include <chrono>


namespace reveal3d {

class Timer {
public:
  Timer() { reset(); }

  [[nodiscard]] static auto now() { return std::chrono::high_resolution_clock::now(); }

  // template<typename T>
  [[nodiscard]] static auto diff(std::chrono::time_point<std::chrono::high_resolution_clock> const& time) {
    return std::chrono::duration<f64>(now() - time).count();
  }

  [[nodiscard]] f64 deltaTime() const { return delta_time_; }

  [[nodiscard]] f64 frameTime() const { return frame_time_; }

  [[nodiscard]] f64 fps() const { return 1.0 / delta_time_; }

  [[nodiscard]] f64 meanFps() const { return static_cast<f64>(total_frames_) / totalTime(); }

  [[nodiscard]] uint64_t averageFps() const { return fps_; }

  [[nodiscard]] uint64_t totalFrames() const { return total_frames_; }

  [[nodiscard]] bool isRunning() const { return !stopped_; }

  [[nodiscard]] f64 diff(f64 const time) const { return totalTime() - time; }

  [[nodiscard]] f64 totalTime() const {
    auto const end_time = stopped_ ? stop_time_ : now();
    return std::chrono::duration<f64>(end_time - base_time_ - paused_duration_).count();
  }

  void reset() {
    base_time_       = now();
    prev_time_       = base_time_;
    stop_time_       = {};
    paused_duration_ = std::chrono::duration<f64>::zero();
    stopped_         = false;
    total_frames_    = 0;
  }

  void start() {
    if (stopped_) {
      auto const start_time = now();
      paused_duration_ += start_time - stop_time_;
      prev_time_ = start_time;
      stopped_   = false;
      stop_time_ = {};
    }
  }

  void stop() {
    if (!stopped_) {
      stop_time_ = now();
      stopped_   = true;
    }
  }

  void tick() {
    auto const curr_time = now();
    total_frames_++;

    delta_time_ = std::chrono::duration<f64>(curr_time - prev_time_).count();
    frame_time_ = delta_time_;
    prev_time_  = curr_time;

    if (auto const elapsed = std::chrono::duration_cast<std::chrono::seconds>(curr_time - base_time_).count();
        total_time_ < elapsed) {
      total_time_        = elapsed;
      fps_               = total_frames_ - prev_total_frames_;
      prev_total_frames_ = total_frames_;
    }

    delta_time_ = std::max(delta_time_, 0.0);
    if (stopped_)
      delta_time_ = 0.0;
  }

  void pause() {
    if (stopped_)
      start();
    else
      stop();
  }

private:
  using clock      = std::chrono::high_resolution_clock;
  using time_point = std::chrono::time_point<clock>;

  time_point base_time_ {};
  time_point stop_time_ {};
  time_point prev_time_ {};
  std::chrono::duration<f64> paused_duration_ {};

  f64 delta_time_        = 0.0;
  f64 frame_time_        = 0.0;
  u64 total_time_        = 0;
  u64 fps_               = 0;
  u64 total_frames_      = 0;
  u64 prev_total_frames_ = 0;
  bool stopped_          = false;
};


} // namespace reveal3d
