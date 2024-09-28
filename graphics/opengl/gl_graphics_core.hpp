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
    void LoadPipeline();
    void LoadAssets();
    void LoadAsset();
    void Update(render::Camera& camera);
    void RenderSurface();
    void Terminate();
    void Resize(const window::Resolution &res);

    INLINE void SetWindow(WHandle wHandle) { window_ = wHandle; }

private:
    void CreateContext();
    void SwapBuffer();
    void TerminateContext();
    math::mat4 passConstant_;
    std::vector<opengl::RenderElement> renderElements_;
    opengl::RenderLayers renderLayers_;
    WHandle window_ {};
};

}

