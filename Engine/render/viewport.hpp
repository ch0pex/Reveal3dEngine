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
struct Viewport {
    explicit Viewport(window::InitInfo &windowInfo) :
        window(windowInfo), renderer(&window.GetRes(), timer) { }
    Window window;
    Renderer<Gfx> renderer;
    Timer timer;
};

} // reveal3d