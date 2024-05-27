/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file viewport.hpp
 * @version 1.0
 * @date 28/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "window/window.hpp"
#include "renderer.hpp"

#include <stdexcept>
#include <iostream>

namespace reveal3d::render {

template<graphics::HRI Gfx, window::Mng<Gfx> Window>
class Viewport {
public:
    explicit Viewport(window::InitInfo &windowInfo) :
        window_(windowInfo), renderer_(&window_.GetRes(), timer_) { }
    INLINE Timer& Time() { return timer_; }
    void Init();
    void Run();
    f64 BenchMark(u32 seconds);
private:
    Window window_;
    Renderer<Gfx> renderer_;
    Timer timer_;
};

template<graphics::HRI Gfx, window::Mng<Gfx> Window>
void Viewport<Gfx, Window>::Init() {
    try {
        window_.Create(renderer_);
        renderer_.Init(window_.GetHandle());
        window_.Show();

    } catch(std::exception &e) {
        renderer_.Destroy();
        log(logERROR) << e.what();
//        MessageBoxA(window_.GetHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
    }
}

template<graphics::HRI Gfx, window::Mng<Gfx> Window>
void Viewport<Gfx, Window>::Run() {
    try {
        timer_.Reset();
        while(!window_.ShouldClose()) {
            timer_.Tick();
            window_.ClipMouse(renderer_);
            renderer_.Update();
            core::scene.Update(timer_.DeltaTime());
#ifdef WIN32
            if constexpr (not std::same_as<Window, window::Win32>)
                renderer_.Render();
#endif
            window_.Update();
        }
        renderer_.Destroy();
    } catch(std::exception &e) {
        renderer_.Destroy();
        log(logERROR) << e.what();
//        MessageBoxA(window_.GetHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
    }
}

template<graphics::HRI Gfx, window::Mng<Gfx> Window>
f64 Viewport<Gfx, Window>::BenchMark(u32 seconds) {
    try {
        timer_.Reset();
        while(!window_.ShouldClose()) {
            if (timer_.TotalTime() > seconds)
                break;
            timer_.Tick();
            window_.ClipMouse(renderer_);
            renderer_.Update();
            core::scene.Update(timer_.DeltaTime());
#ifdef WIN32
            if constexpr (not std::same_as<Window, window::Win32>)
                renderer_.Render();
#endif
            window_.Update();
        }
        renderer_.Destroy();
    } catch(std::exception &e) {
        renderer_.Destroy();
        log(logERROR) << e.what();
        //        MessageBoxA(window_.GetHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
    }
    return timer_.AverageFps();
}
} // reveal3d