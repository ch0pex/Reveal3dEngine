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
    math::vec3 pos;
    math::vec3 normal;
    math::vec3 color;
    math::vec2 tex1;
    math::vec2 tex2;
};

class Mesh {

};

}
