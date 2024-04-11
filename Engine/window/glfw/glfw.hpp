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
 * Longer description
 */

#pragma once

#include "render/renderer.hpp"
#include "window/window_info.hpp"
#include "input/input.hpp"

namespace reveal3d::window {


class Glfw {
public:
    Glfw(InitInfo &info);

    template<graphics::HRI Gfx> void Create(render::Renderer<Gfx> &renderer);
    //    i32 Run(Renderer<Gfx> &renderer);
    void Show();
    void Update();
    void CloseWindow(input::action act, input::type type);
    template<graphics::HRI Gfx> void ClipMouse(render::Renderer<Gfx> &renderer);
    bool ShouldClose();

};

}

