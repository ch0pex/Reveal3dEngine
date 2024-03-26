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
#include "transform.hpp"

namespace reveal3d::core {

class Scene;

struct Entity {
    Transform *transform { nullptr };
    Geometry *geometry { nullptr };
};

class Scene {
public:
    Scene() = default;

    Entity CreateEntity();
    Entity GetEntity();
    void AddEntity(Entity &entity);

    INLINE std::vector<u32>& Transforms();
    INLINE std::vector<u32>& Geometries();

private:
    std::vector<Transform> transforms_;
    std::vector<Geometry> geometries_;
    //std::vector<Script> scripts_;
//    std::vector<Light> lights;
};

}
