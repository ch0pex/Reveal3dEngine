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

#include "primitive_types.hpp"
#include "platform.hpp"
#include "input/input.hpp"
#include <chrono>


namespace reveal3d {

class Timer {
public:
    Timer();

    [[nodiscard]] f32 totalTime() const;
    [[nodiscard]] f32 deltaTime() const { return static_cast<f32>(delta_time_); }
    [[nodiscard]] f32 frameTime() const { return static_cast<f32>(frame_time_); }
    [[nodiscard]] f32 fps() const { return static_cast<f32>(1 / delta_time_); }
    [[nodiscard]] f64 meanFps() const { return static_cast<f64>(total_frames_) / static_cast<f64>(totalTime()); }
    [[nodiscard]] f64 averageFps() const { return fps_; }
    [[nodiscard]] u64 totalFrames() const { return total_frames_; }
    [[nodiscard]] bool isRunning() { return !stopped_; };
    [[nodiscard]] f32 diff(f32 time) const;

    void reset();
    void start();
    void stop();
    void tick();
    void pause(input::Action act, input::type type);

private:

#ifdef _WIN32
    static inline void queryFrequency(i64 &time) { QueryPerformanceFrequency((LARGE_INTEGER *) &time); }
    static inline void queryCounter(i64 &time) { QueryPerformanceCounter((LARGE_INTEGER *) &time); }
#else
    static inline void QueryFrequency(i64 &time) { time = 1000000000; }
    static inline void QueryCounter(i64 &time) {
        auto now = std::chrono::high_resolution_clock::now();
        auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
        ime = now_ns.time_since_epoch().count();
    }

#endif
    f64 second_per_count_;
    i64 counts_per_second_;
    f64 delta_time_; // Affected by pause
    f64 frame_time_; // Non afected by pause

    i64 base_time_;
    i64 paused_time_;
    i64 stop_time_;
    i64 prev_time_;
    i64 curr_time_;

    u64 total_time_;
    u64 fps_;
    u64 total_frames_;
    u64 prev_total_frames_;
    bool stopped_;

};


}



