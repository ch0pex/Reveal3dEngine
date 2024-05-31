/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file window.hpp
 * @version 1.0
 * @date 27/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#ifdef WIN32
#include "win32/win32.hpp"
#endif
#include "glfw/glfw.hpp"

#include "window_info.hpp"
#include <concepts>

namespace reveal3d::window {

template<typename T, typename Gfx>
concept Mng = requires(T window, render::Renderer<Gfx> renderer) {
    {window.Create(renderer)} ->  std::same_as<void>;
    {window.Show()} ->  std::same_as<void>;
    {window.Update()} ->  std::same_as<void>;
    {window.ShouldClose()} ->  std::same_as<bool>;
    {window.GetRes()} ->  std::same_as<Resolution&>;
    {window.GetHandle()} ->  std::same_as<WHandle>;
};

}