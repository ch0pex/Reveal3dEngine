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

#include "vertex.hpp"
#include "material.hpp"

#include <vector>

namespace reveal3d::render {

struct SubMesh {
    Shader shader       { opaque };
    u32 vertexPos       { 0 };
    u32 indexPos        { 0 };
    u32 indexCount      { 0 };
    bool visible        { true };
};

struct Mesh {
    std::vector<render::Vertex> vertices;
    std::vector<u32> indices;
};

}

