/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file renderer.hpp
 * @version 1.0
 * @date 25/02/2024
 * @brief render class
 *
 * Renderer template class, abstracts at a very high level the HRI (Hardware render interface)
 *
 */

#pragma once

#include "camera.hpp"
#include "graphics/gfx.hpp"

#include <functional>
#include "IMGUI/backends/imgui_impl_dx12.h"
#include "IMGUI/backends/imgui_impl_win32.h"

namespace reveal3d::render {

template<graphics::HRI Gfx>
class Renderer {
public:
    Renderer(window::Resolution* res, Timer& timer);
    void init(WHandle w_handle);
    void update();
    void render();
    void destroy();
    void resize(const window::Resolution& res);

    Gfx& graphics() { return graphics_; }

    inline f32 deltaTime() const { return timer_.deltaTime(); }
    inline void cameraResetMouse() { camera_.resetMouse(); }
    inline Timer& time() { return timer_; }

private:
    Gfx graphics_;
    Camera camera_;
    Timer& timer_;
};

template<graphics::HRI Gfx>
Renderer<Gfx>::Renderer(window::Resolution* res, Timer& timer) : camera_(*res), graphics_(res), timer_(timer) {}

template<graphics::HRI Gfx>
void Renderer<Gfx>::init(WHandle w_handle) {
    f32 time = timer_.totalTime();
    graphics_.initWindow(w_handle);
    graphics_.loadPipeline();
    logger(LogDebug) << "Initializing Pipeline...[" << timer_.diff(time) * 1000 << "ms]";
    time = timer_.totalTime();
    graphics_.loadAssets();
    logger(LogDebug) << "Loading assets...[" << timer_.diff(time) * 1000 << "ms]";
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::update() {
    camera_.update(timer_);
    graphics_.update(camera_);
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::render() {
    graphics_.renderSurface();
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::destroy() {
    logger(LogDebug) << "Cleaning pipeline...[" << timer_.totalTime() << "]";
    graphics_.terminate();
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::resize(const window::Resolution& res) {
    camera_.resize(res);
    graphics_.resize(res);
}
} // namespace reveal3d::render
