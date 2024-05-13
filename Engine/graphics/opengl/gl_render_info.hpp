/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file gl_render_info.hpp
 * @version 1.0
 * @date 10/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"
#include "math/math.hpp"
#include "render/vertex.hpp"

#include <vector>

namespace reveal3d::graphics::opengl {

struct RenderElement {
    RenderElement(std::vector<render::Vertex> &vertices, std::vector<u16> &indices, math::mat4 world);
    u32 vao;
    u32 vbo;
    u32 ebo;
    math::mat4 world;
};

}

