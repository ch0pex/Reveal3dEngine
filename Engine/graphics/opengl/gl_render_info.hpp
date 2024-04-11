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

namespace reveal3d::graphics::opengl {

struct RenderInfo {
   u32 vao;
   u32 vbo;
   u32 ebo;
   u32 faces;
   math::mat4 world;
};

}

