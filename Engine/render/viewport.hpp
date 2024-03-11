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

template<graphics::HRI Gfx, template<typename> class Window>
// requires,
class Viewport {
public:
    explicit Viewport(window::InitInfo &info) : renderer_(info.res), window_(info) {}
    INLINE void Run() { window_.Run(renderer_); }
    void Pause() {
        //renderer_.Pause();
    }
    void Destroy() {
        renderer_.Destroy();
        //window_.Close();
    }
private:
    Renderer<Gfx> renderer_;
    Window<Gfx> window_;
    //Scene &scene_
};

} // reveal3d
