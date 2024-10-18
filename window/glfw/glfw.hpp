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
 * GLFW backend manager class
 */

#pragma once

#include "render/renderer.hpp"
#include "window/window_info.hpp"
#include "input/input.hpp"

#include "GLFW/glfw3.h"
#include <glfw/glfw3native.h>


namespace reveal3d::window {


class Glfw {
public:
    Glfw(Config &info);

    template<graphics::HRI Gfx> void Create(render::Renderer<Gfx> &renderer);
    void Show();
    template<graphics::HRI Gfx> void Update(render::Renderer<Gfx> &renderer);
    void CloseWindow(input::action act, input::type type);
    bool ShouldClose();

    [[nodiscard]] inline Resolution& GetRes() { return info_.res; }
    [[nodiscard]] inline WHandle GetHandle() const { return info_.handle; }
private:
    template<graphics::HRI Gfx> void ClipMouse(render::Renderer<Gfx> &renderer);

    Config info_;
    GLFWwindow* winPtr_;

};

template<graphics::HRI Gfx>
void Glfw::Update(render::Renderer<Gfx> &renderer) {
    //Handle inputs
    glfwPollEvents();
}

template<graphics::HRI Gfx>
void Glfw::Create(render::Renderer<Gfx> &renderer) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef WIN32
    winPtr_ = glfwCreateWindow(1920, 1080, "CraftGL", NULL, NULL);
    if (!winPtr_) {
        std::cout << "Creat window error\n";
    }
    info_.handle.hwnd = glfwGetWin32Window(winPtr_);

    if (info_.handle.hwnd == NULL) {
        glfwTerminate();
        std::terminate();
    }

#else
    winPtr_ = glfwCreateWindow(1920, 1080, "CraftGL", NULL, NULL);
    info_.handle = winPtr_;
    if (!info_.handle) {
        glfwTerminate();
        std::terminate();
    }
#endif
    glfwSwapInterval( 0 );

}

template<graphics::HRI Gfx>
void Glfw::ClipMouse(render::Renderer<Gfx> &renderer) {

}

}

