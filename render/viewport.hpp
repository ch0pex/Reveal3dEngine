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
 * Viewport template class, it takes two classes as parameters a window manager
 * and a renderer  
 *                                 ----------
 *                                | Viewport |
 *                                 ----------
 *                                     |
 *                  -----------------------------------------
 *                  |                                       |
 *              ----------                              ----------
 *             | Renderer |                           | Window Mng |
 *              ----------                              ----------
 *                  |                                       |
 *         -------------------                   -------------------------
 *         |                 |                   |                       |
 *   ----------          ----------         ----------               ----------
 *  |   Dx12   |        |  OpenGL  |       |  Win32   |             |   GLFW   |
 *   ----------          ----------         ----------               ----------
 *
 */

#pragma once

#include "window/window.hpp"
#include "renderer.hpp"

#include <stdexcept>
#include <iostream>

namespace reveal3d::render {

template<graphics::HRI Gfx, window::Mng<Gfx> Window>
struct Viewport {
    explicit Viewport(window::InitInfo &windowInfo) : window(windowInfo), renderer(&window.GetRes(), timer) { }
    void Init();
    void Run();
    f64 BenchMark(u32 seconds);
    INLINE Timer& Time() { return timer; }

    Window window;
    Renderer<Gfx> renderer;
    Timer timer;
};

template<graphics::HRI Gfx, window::Mng<Gfx> Window>
void Viewport<Gfx, Window>::Init() {
    try {
        window.Create(renderer);
        renderer.Init(window.GetHandle());
        window.Show();

    } catch(std::exception &e) {
        renderer.Destroy();
        log(logERROR) << e.what();
//        MessageBoxA(window.GetHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
    }
}

template<graphics::HRI Gfx, window::Mng<Gfx> Window>
void Viewport<Gfx, Window>::Run() {
    try {
        timer.Reset();
        while(!window.ShouldClose()) {
            timer.Tick();
            window.ClipMouse(renderer);
            renderer.Update();
            core::scene.Update(timer.DeltaTime());
#ifdef WIN32
            if constexpr (not std::same_as<Window, window::Win32>)
                renderer.Render();
#else
            renderer.Render();
#endif
            window.Update();
        }
        renderer.Destroy();
    } catch(std::exception &e) {
        renderer.Destroy();
        log(logERROR) << e.what();
        MessageBoxA(window.GetHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
    }
}

template<graphics::HRI Gfx, window::Mng<Gfx> Window>
f64 Viewport<Gfx, Window>::BenchMark(u32 seconds) {
    try {
        timer.Reset();
        while(!window.ShouldClose()) {
            if (timer.TotalTime() > seconds)
                break;
            timer.Tick();
            window.ClipMouse(renderer);
            renderer.Update();
            core::scene.Update(timer.DeltaTime());
#ifdef WIN32
            if constexpr (not std::same_as<Window, window::Win32>)
                renderer.Render();
#endif
            window.Update();
        }
        renderer.Destroy();
    } catch(std::exception &e) {
        renderer.Destroy();
        log(logERROR) << e.what();
        MessageBoxA(window.GetHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
    }
    return timer.MeanFps();
}
} // reveal3d