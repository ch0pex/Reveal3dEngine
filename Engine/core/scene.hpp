/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file scene.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "geometry.hpp"
#include "transform.hpp"
#include "render/light.hpp"

#include <vector>

namespace reveal3d::core {


class Entity {

};

struct Scene {
    //std::vector<Light> lights;
    std::vector<Transform> transforms;
    std::vector<Geometry> geometries;

};

}
