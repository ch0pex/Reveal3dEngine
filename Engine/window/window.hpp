/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file window_.hpp
 * @version 1.0
 * @date 27/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#ifdef WIN32
#include "win32/win_32.hpp"
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
};

}