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
    math::vec3 pos { 0.0f, 0.0f, 0.0f};
    math::vec4 color { 0.4f, 0.4f, 0.4f, 0.0f};
    math::vec3 normal { 0.0f, 0.0f, 0.0f};
    math::vec2 uv { 0.0f, 0.0f};
};

}
