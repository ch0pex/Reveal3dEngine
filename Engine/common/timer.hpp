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

namespace reveal3d {

class Timer {
public:
    Timer();

    [[nodiscard]] f32 TotalTime() const;
    [[nodiscard]] INLINE f32 DeltaTime() const { return static_cast<f32>(deltaTime_); }
    [[nodiscard]] INLINE f32 Fps() const { return static_cast<f32>(1 / deltaTime_); }

    void Reset();
    void Start();
    void Stop();
    void Tick();
    f32 Diff(f32 time) const;

private:

#ifdef _WIN32
    static INLINE void QueryFrequency(i64 &time) { QueryPerformanceFrequency((LARGE_INTEGER *) &time); }
    static INLINE void QueryCounter(i64 &time) { QueryPerformanceCounter((LARGE_INTEGER *) &time); }
#else

#endif

    f64 secondPerCount_;
    i64 countsPerSecond_;
    f64 deltaTime_;

    i64 baseTime_;
    i64 pausedTime_;
    i64 stopTime_;
    i64 prevTime_;
    i64 currTime_;

    bool stopped_;
};


}



