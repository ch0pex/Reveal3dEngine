/************************************************************************
* Copyright (c) 2024 Alvaro Cabrera Barrio
* This code is licensed under MIT license (see LICENSE.txt for details)
************************************************************************/
/**
* @file entity.hpp
* @version 1.0
* @date 11/03/2024
* @brief Short description
*
* Longer description
*/

#include "common/common.hpp"
#include "math/math.hpp"
#include "scene.hpp"

namespace reveal3d::core {


class Entity {
public:
    Entity(u32 id, Scene& scene) : id_(id), scene_(scene) {}
    INLINE void SetPosition(f32 x, f32 y, f32 z) { scene_.GetTransform(id_).SetPosition({x, y, z}); }
    INLINE void SetPosition(math::vec3& pos) { scene_.GetTransform(id_).SetPosition(pos); }

    INLINE void SetRotation(f32 x, f32 y, f32 z) { scene_.GetTransform(id_).SetRotation({x, y, z}); }
    INLINE void SetRotation(math::vec3& rot) { scene_.GetTransform(id_).SetRotation(rot); }

    INLINE void SetScale(f32 s) { scene_.GetTransform(id_).SetScale({s}); }
    INLINE void SetScale(f32 x, f32 y, f32 z) { scene_.GetTransform(id_).SetScale({x, y, z}); }
    INLINE void SetScale(math::vec3& scale) { scene_.GetTransform(id_).SetScale(scale); }

    INLINE void SetTransform(Transform& transform);
    INLINE void SetGeometry(Geometry& geometry);

private:
    Scene &scene_;
    u32 id_;
};


}