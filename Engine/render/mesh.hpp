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
#include "graphics/directX12/render_info.hpp"

#include <vector>



namespace reveal3d::render {

enum shader : u8 {
    opaque = 0,
    flat, // Not affected by lighting
//    transparent,

    count
};

struct Mesh {
    graphics::RenderInfo* renderInfo { nullptr }; // Vertex buffer where mesh is
    shader shader { opaque };
    bool visible { true };
    u32 vertexPos { 0 };
    u32 indexPos { 0 };
    u32 indexCount { 0 };
};

}

