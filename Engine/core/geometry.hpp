/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file geometry.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "render/mesh.hpp"

#include <vector>

namespace reveal3d::core {

/*TODO: move this to Mesh and have multiple meshes in an entity geometry? */ class Geometry {
public:
    INLINE u32 VertexCount() { return vertices_.size(); }
    INLINE u32 IndexCount() {return indices_.size(); }
    INLINE std::vector<render::Vertex>* GetVertices() { return &vertices_; }
    INLINE std::vector<u16>* GetIndices() { return &indices_; }
private:
    std::vector<render::Vertex> vertices_;
    std::vector<u16> indices_;
};


}
