/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file mesh.hpp
 * @version 1.0
 * @date 10/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "math/math.hpp"


namespace reveal3d::render {

struct Vertex {
//    Vertex() : pos(0.0f, 0.0f, 0.0f), color(0.3f, 0.3f, 0.3f, 0.0f) {}
//    Vertex(f32 x, f32 y, f32 z) : pos(x,y,z), color(0.2f, 0.2f, 0.2f, 0.0f) {}
    math::vec3 pos;
    math::vec4 color;
};

}
