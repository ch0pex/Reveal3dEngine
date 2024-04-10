/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file graphics_core.cpp
 * @version 1.0
 * @date 05/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "gl_graphics_core.hpp"

namespace reveal3d::graphics::opengl {

Graphics::Graphics(window::Resolution *res) {

}

void Graphics::LoadPipeline() {
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Error on OpenGl init\r\n");
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    renderLayers_.Init();

}

void Graphics::LoadAssets() {

}

void Graphics::LoadAsset() {

}

void Graphics::Update(render::Camera &camera) {

}

void Graphics::PrepareRender() {

}

void Graphics::Draw() {

}

void Graphics::Terminate() {

}

void Graphics::Resize(window::Resolution &res) {

}

}
