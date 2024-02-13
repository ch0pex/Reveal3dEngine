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


using namespace reveal3d::render;

namespace reveal3d::window {

template<typename Gfx>
class Win32 {
public:
    Win32(InitInfo &info) : info_(info) {}

    i32 Run(Renderer<Gfx> &renderer);
    [[nodiscard]] WHandle GetHwnd() const;

protected:
    static LRESULT CALLBACK WindowProc(WHandle hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    void InitWindow(Renderer<Gfx> &renderer);

    InitInfo info_;
};

}