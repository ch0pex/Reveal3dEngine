/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file primitives.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "core/components/geometry.hpp"

namespace reveal3d::content {

u16 getCubeData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices);
u16 getPlaneData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices);
u16 getSphereData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices);
u16 getCylinderData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices);
u16 getConeData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices);
u16 getTorusData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices);

}

