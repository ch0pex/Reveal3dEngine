/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file win_32.hpp
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
#include <WindowsX.h>

using namespace reveal3d::render;

namespace reveal3d::window {

template<typename Gfx>
class Win32 {
public:
    Win32(InitInfo &info);

    void Create(Renderer<Gfx> &renderer);
//    i32 Run(Renderer<Gfx> &renderer);
    void CloseWindow(input::action act, input::type type);
    void ClipMouse(Renderer<Gfx> &renderer);

    [[nodiscard]] INLINE Resolution& GetRes() { return info_.res; }
    [[nodiscard]] INLINE WHandle GetHwnd() const { return info_.windowHandle; }
private:
    input::System<Win32> inputSystem_;
    InitInfo info_;

};

template<typename Gfx>
Win32<Gfx>::Win32(InitInfo &info) : info_(info)
{
   inputSystem_.AddHandlerDown(input::action::window_close, {&Win32::CloseWindow, nullptr, this});
}

}