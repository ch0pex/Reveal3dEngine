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
 *             | Renderer |                           | Window Manager |
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

#include "renderer.hpp"
#include "window/window.hpp"

#include <iostream>
#include <stdexcept>

namespace reveal3d::render {

template<graphics::HRI Gfx, window::Manager<Gfx> Window>
struct Viewport {
  explicit Viewport(const window::Info& windowInfo) : window(windowInfo), renderer(&window.getRes(), timer) { }

  void init() try {
    window.create(renderer);
    renderer.init(window.getHandle());
    window.show();
  }
  catch (std::exception& e) {
    renderer.destroy();
    logger(LogError) << e.what();
  };

  void run();
  f64 benchMark(u32 seconds);
  Timer& time() { return timer; }

  Window window;
  Renderer<Gfx> renderer;
  Timer timer;
};

template<graphics::HRI Gfx, window::Manager<Gfx> Window>
void Viewport<Gfx, Window>::run() {
  try {
    timer.reset();
    while (!window.shouldClose()) {
      timer.tick();
      window.clipMouse(renderer);
      renderer.update();
      core::scene.update(timer.deltaTime());
#ifdef WIN32
      if constexpr (not std::same_as<Window, window::Win32>)
        renderer.render();
#else
      renderer.Render();
#endif
      window.update();
    }
    renderer.destroy();
  }
  catch (std::exception& e) {
    renderer.destroy();
    logger(LogError) << e.what();
    MessageBoxA(window.getHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
  }
}

template<graphics::HRI Gfx, window::Manager<Gfx> Window>
f64 Viewport<Gfx, Window>::benchMark(u32 seconds) {
  try {
    timer.reset();
    while (!window.shouldClose()) {
      if (timer.totalTime() > seconds)
        break;
      timer.tick();
      window.clipMouse(renderer);
      renderer.update();
      core::scene.update(timer.deltaTime());
#ifdef WIN32
      if constexpr (not std::same_as<Window, window::Win32>)
        renderer.render();
#endif
      window.update();
    }
    renderer.destroy();
  }
  catch (std::exception& e) {
    renderer.destroy();
    logger(LogError) << e.what();
    MessageBoxA(window.getHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
  }
  return timer.meanFps();
}
} // namespace reveal3d::render
