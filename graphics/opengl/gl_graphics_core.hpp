/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file graphics_core.hpp
 * @version 1.0
 * @date 05/04/2024
 * @brief Short description
 *
 * Longer description
 */
#pragma once

#include "gl_render_info.hpp"
#include "render/camera.hpp"

#include "gl_render_layers.hpp"

namespace reveal3d::graphics {

class OpenGL {
public:
    explicit OpenGL(window::Resolution *res);
    void loadPipeline();
    void loadAssets();
    void loadAsset();
    void update(render::Camera &camera);
    void renderSurface();
    void terminate();
    void resize(const window::Resolution &res);

    inline void initWindow(WHandle w_handle) { window_ = w_handle; }

private:
    void createContext();
    void swapBuffer();
    void terminateContext();
    math::mat4 pass_constant_;
    std::vector<opengl::RenderElement> render_elements_;
    opengl::RenderLayers render_layers_;
    WHandle window_ {};
};

}

