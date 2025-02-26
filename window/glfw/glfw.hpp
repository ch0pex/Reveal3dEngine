/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file glfw.hpp
 * @version 1.0
 * @date 27/02/2024
 * @brief Short description
 *
 * GLFW backend manager class
 */

#pragma once

#include "input/input.hpp"
#include "render/renderer.hpp"
#include "window/window_info.hpp"

#include "GLFW/glfw3.h"


namespace reveal3d::window {


class Glfw {
public:
  explicit Glfw(Info const& info);

  template<graphics::HRI Gfx>
  void create(render::Renderer<Gfx>& renderer);
  void show();
  template<graphics::HRI Gfx>
  void update(render::Renderer<Gfx>& renderer);
  void closeWindow(input::Action act, input::type type);
  bool shouldClose();

  [[nodiscard]] Resolution getRes() const { return info_.res; }
  [[nodiscard]] WHandle getHandle() const { return info_.handle; }

private:
  template<graphics::HRI Gfx>
  void clipMouse(render::Renderer<Gfx>& renderer);

  Info info_;
  GLFWwindow* window_pointer_;
};

template<graphics::HRI Gfx>
void Glfw::update(render::Renderer<Gfx>& renderer) {
  // Handle inputs
  glfwPollEvents();
}

template<graphics::HRI Gfx>
void Glfw::create(render::Renderer<Gfx>& renderer) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef WIN32
  win_ptr_ = glfwCreateWindow(1920, 1080, "CraftGL", NULL, NULL);
  if (!win_ptr_) {
    logger(LogError) << "Create window error\n";
  }
  info_.handle.hwnd = glfwGetWin32Window(win_ptr_);

  if (info_.handle.hwnd == nullptr) {
    glfwTerminate();
    std::terminate();
  }

#else
  window_pointer_ = glfwCreateWindow(1920, 1080, "CraftGL", NULL, NULL);
  info_.handle    = window_pointer_;
  if (!info_.handle) {
    glfwTerminate();
    std::terminate();
  }
#endif
  glfwSwapInterval(0);
}

template<graphics::HRI Gfx>
void Glfw::clipMouse(render::Renderer<Gfx>& renderer) { }

} // namespace reveal3d::window
