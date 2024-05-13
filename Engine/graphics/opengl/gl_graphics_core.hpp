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

#include "GL/glew.h"
#include "gl_render_layers.hpp"

namespace reveal3d::graphics::opengl {

class Graphics {
public:
    explicit Graphics(window::Resolution *res);
    void LoadPipeline();
    void LoadAssets();
    void LoadAsset();
    void Update(render::Camera& camera);
    void PrepareRender();
    void Draw();
    void Terminate();
    void Resize(const window::Resolution &res);

    INLINE void SetWindow(WHandle wHandle) { window_ = wHandle; }

private:
    void CreateContext();
    void SwapBuffer();
    void TerminateContext();
    math::mat4 passConstant_;
    std::vector<RenderElement> renderElements_;
    RenderLayers renderLayers_;
    WHandle window_ {};
};

}

