/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file renderer.hpp
 * @version 1.0
 * @date 25/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <functional>
#include "camera.hpp"
#include "core/scene.hpp"
#include "graphics/gfx.hpp"

#include "IMGUI/backends/imgui_impl_win32.h"
#include "IMGUI/backends/imgui_impl_dx12.h"

namespace reveal3d::render {

template<graphics::HRI Gfx>
class Renderer {
public:
    Renderer(window::Resolution *res, Timer &timer);
    void Init(WHandle wHandle);
    void Update();
    void Render();
    void Destroy();
    void Resize(const window::Resolution &res);

    Gfx& Graphics() { return graphics_; }

    INLINE f32 DeltaTime() const { return timer_.DeltaTime(); }
    INLINE  void CameraResetMouse() { camera_.ResetMouse(); }
    INLINE Timer& Time() { return timer_; }

private:
    Gfx graphics_;
    Camera camera_;
    Timer& timer_;
};

template<graphics::HRI Gfx>
Renderer<Gfx>::Renderer(window::Resolution *res, Timer &timer)
        : camera_(*res),
          graphics_(res),
          timer_(timer)
{

}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Init(WHandle wHandle) {
    f32 time = timer_.TotalTime();
    graphics_.SetWindow(wHandle);
    graphics_.LoadPipeline();
    log(logDEBUG) << "Initializing Pipeline...[" << timer_.Diff(time) * 1000 <<"ms]";
    time = timer_.TotalTime();
    graphics_.LoadAssets();
    log(logDEBUG) << "Loading assets...[" << timer_.Diff(time) * 1000 <<"ms]";
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Update() {
    camera_.Update(timer_);
    graphics_.Update(camera_);
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Render() {
    graphics_.PrepareRender();
    graphics_.Draw();
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Destroy() {
    log(logDEBUG) << "Cleaning pipeline...[" << timer_.TotalTime()  <<"]";
    graphics_.Terminate();
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Resize(const window::Resolution &res) {
    camera_.Resize(res);
    graphics_.Resize(res);
}
}