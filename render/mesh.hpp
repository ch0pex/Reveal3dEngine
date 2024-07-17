/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file mesh.hpp
 * @version 1.0
 * @date 10/03/2024
 * @brief Mesh and Submesh 
 *
 * Mesh, submesh and shader structures 
 */

#pragma once

#include "math/math.hpp"
#include "vertex.hpp"

#include <vector>

namespace reveal3d::render {

enum Shader : u8 {
    opaque = 0,
    flat, // Not affected by lighting
    grid,
//    transparent,

    count
};

struct SubMesh {
    u32 renderInfo      { UINT_MAX };
    u32 constantIndex   { 0 };
    Shader shader       { opaque };
    u32 vertexPos       { 0 };
    u32 indexPos        { 0 };
    u32 indexCount      { 0 };
    bool visible        { true };
};

struct Mesh {
    std::vector<render::Vertex> vertices_;
    std::vector<u32> indices_;
    u32 submeshCount { 1 };
    u32 renderInfo { UINT_MAX }; // Vertex buffer where mesh is
};

}

