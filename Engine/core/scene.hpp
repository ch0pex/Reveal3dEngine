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
    Transform *transform;
    Geometry *geometry;
};

class Scene {
public:
    Scene() = default;

    Entity CreateEntity();
    void AddEntity(Entity &entity);

    INLINE std::vector<Transform>& Transforms() { return transforms_; }
    INLINE std::vector<Geometry>& Geometries() { return geometries_; }
    INLINE u32 NumEntities() { return entities_; }

private:
    std::vector<Transform> transforms_;
    std::vector<Geometry> geometries_;
    u32 entities_ { 0 };
    //std::vector<Script> scripts_;
//    std::vector<Light> lights;
};

}
