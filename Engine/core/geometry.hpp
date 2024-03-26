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

class Geometry {
public:
    INLINE u32 IndexCount() {return indexBuffer_.size(); }
private:
    std::vector<render::Vertex> vertexBuffer_;
    std::vector<u32> indexBuffer_;
};

}
