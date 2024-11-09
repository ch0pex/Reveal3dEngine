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

namespace reveal3d::render {

template<graphics::HRI Gfx>
class Renderer {
public:
  explicit Renderer(window::Resolution* res) : graphics_(res), camera_(*res) { }

  void init(WHandle w_handle) {
    f32 time = timer_.totalTime();
    graphics_.initWindow(w_handle);
    graphics_.loadPipeline();
    logger(LogInfo) << "Initializing Pipeline...[" << timer_.diff(time) * 1000 << "ms]";
    time = timer_.totalTime();
    graphics_.loadAssets();
    logger(LogInfo) << "Loading assets...[" << timer_.diff(time) * 1000 << "ms]";
  }

  void update() {
    camera_.update(timer_);
    graphics_.update(camera_);
  }

  void render() { graphics_.renderSurface(); }

  void destroy() {
    logger(LogInfo) << "Cleaning pipeline...[" << timer_.totalTime() << "]";
    graphics_.terminate();
  }

  void resize(window::Resolution const& res) {
    camera_.resize(res);
    graphics_.resize(res);
  }

  Gfx& graphics() { return graphics_; }

  [[nodiscard]] f32 deltaTime() const { return timer_.deltaTime(); }
  [[nodiscard]] Timer& time() { return timer_; }

  void cameraResetMouse() { camera_.resetMouse(); }

private:
  Gfx graphics_;
  Camera camera_;
  Timer timer_;
};

} // namespace reveal3d::render
