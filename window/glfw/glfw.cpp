/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file glfw.cpp
 * @version 1.0
 * @date 30/04/2024
 * @brief Short description
 *
 * Longer description
 */


#include "glfw.hpp"

namespace reveal3d::window {

Glfw::Glfw(Info const& info) { }

void Glfw::show() {
  // show window
}


void Glfw::closeWindow(input::Action act, input::type type) {
  // Close Window
}

bool window::Glfw::shouldClose() { return glfwWindowShouldClose(win_ptr_); }

} // namespace reveal3d::window
