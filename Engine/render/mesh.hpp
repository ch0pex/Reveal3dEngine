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

#include <vector>

class RenderInfo;

namespace reveal3d::render {

enum shader : u8 {
    opaque,
    flat, // Not affected by lighting
    transparent,

    count
};

struct Vertex {
    math::vec3 pos { 0.0f, 0.0f, 0.0f};
    math::vec4 color { 0.4f, 0.4f, 0.4f, 0.0f};
    math::vec3 normal { 0.0f, 0.0f, 0.0f};
    math::vec2 uv { 0.0f, 0.0f};
};

struct Mesh {
    RenderInfo* renderInfo { nullptr};
    shader shader { opaque };
    bool visible { true };
    u32 index;
    u32 size;
};


}

