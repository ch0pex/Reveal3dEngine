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

Glfw::Glfw(InitInfo &info) {

}

void Glfw::Show() {
    //Show window
}


void Glfw::CloseWindow(input::action act, input::type type) {
    //Close Window
}

bool window::Glfw::ShouldClose() {
    return glfwWindowShouldClose(winPtr_);
}

}