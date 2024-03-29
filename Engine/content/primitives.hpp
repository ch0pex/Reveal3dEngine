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

#include "core/geometry.hpp"

namespace reveal3d::content {

void GetCubeData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices);
void GetPlaneData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices);
void GetSphereData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices);
void GetCylinderData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices);
void GetConeData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices);
void GetTorusData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices);

}

