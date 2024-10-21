/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file win32.cpp
 * @version 1.0
 * @date 30/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "win32.hpp"

namespace reveal3d::window {

Win32::Win32(Config &info) : info_(info)
{
    input::add_handler_down(input::Action::WindowClose,
                            {[this](input::Action act, input::type type) { closeWindow(act, type); }});
}

void Win32::show() {
    ShowWindow(info_.handle.hwnd, SW_SHOW);
    UpdateWindow(info_.handle.hwnd);
    is_running_ = true;
}

bool Win32::shouldClose() {
    return !(is_running_);
}

void Win32::closeWindow(input::Action act, input::type type) {
    PostMessage(info_.handle.hwnd, WM_CLOSE, 0, 0);
}

}
