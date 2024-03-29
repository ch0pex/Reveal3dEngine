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
#include "content/primitives.hpp"
#include "transform.hpp"

namespace reveal3d::core {

class Scene;

struct EntityInfo {
    Transform transform;
    Geometry geometry;
};

//TODO: ids, generation and custom std::vector
class Scene {
public:
    Scene() = default;

    void AddEntity(EntityInfo &entity);
    void AddPrimitive(Geometry::primitive type);
    void AddEntityFromObj(const wchar_t *path);

    INLINE Transform &GetTransform(u32 id) { return transforms_.at(id); }
    INLINE Geometry &GetGeometry(u32 id) { return geometries_.at(id); }
    INLINE u32 NumEntities() { return entities_; }
    INLINE std::vector<Transform>& Transforms() { return transforms_; }
    INLINE std::vector<Geometry>& Geometries() { return geometries_; }

private:
//    std::vector<u8> generationId_;
    std::vector<Transform> transforms_;
    std::vector<Geometry> geometries_;
    u32 entities_ { 0 };

//    std::vector<Script> scripts_;
//    std::vector<Light> lights;
};

}
