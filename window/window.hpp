/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file window.hpp
 * @version 1.0
 * @date 27/02/2024
 * @brief Window Concept
 *
 * This C++ Concept defines the constrains that a Window Manager Class
 * must have in order to be used in a Viewport class
 */

#pragma once

#ifdef WIN32
#include "win32/win32.hpp"
#endif
#include "glfw/glfw.hpp"

#include <concepts>
#include "window_info.hpp"

namespace reveal3d::window {

template<typename T, typename Gfx>
concept Manager = requires(T window, render::Renderer<Gfx> renderer) {
  { window.create(renderer) } -> std::same_as<void>;
  { window.show() } -> std::same_as<void>;
  { window.update(renderer) } -> std::same_as<void>;
  { window.shouldClose() } -> std::same_as<bool>;
  { window.getRes() } -> std::same_as<Resolution&>;
  { window.getHandle() } -> std::same_as<WHandle>;
};

} // namespace reveal3d::window
