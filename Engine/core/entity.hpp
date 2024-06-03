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

#pragma once

#include "common/common.hpp"
#include "math/math.hpp"
#include "scene.hpp"

namespace reveal3d::core {


class Entity {
public:
    Entity(); // TODO: Change -1 for invalid when custom id
    explicit Entity(u32 id);
    INLINE void SetPosition(f32 x, f32 y, f32 z) { scene.GetTransform(id_).SetPosition({x, y, z}); }
    INLINE void SetPosition(math::xvec3 pos) { scene.GetTransform(id_).SetPosition(pos); }

    INLINE void SetRotation(f32 x, f32 y, f32 z) { scene.GetTransform(id_).SetRotation({x, y, z}); }
    INLINE void SetRotation(math::xvec3 rot) { scene.GetTransform(id_).SetRotation(rot); }

    INLINE void SetScale(f32 s) { scene.GetTransform(id_).SetScale({s}); }
    INLINE void SetScale(f32 x, f32 y, f32 z) { scene.GetTransform(id_).SetScale({x, y, z}); }
    INLINE void SetScale(math::vec3& scale) { scene.GetTransform(id_).SetScale(scale); }
    INLINE void SetScale(math::xvec3 scale) { scene.GetTransform(id_).SetScale(scale); }

//    INLINE void SetTransform(Transform& transform);
//    INLINE void SetGeometry(Geometry& geometry);

    INLINE std::string& GetName() { return scene.GetName(id_); }
    INLINE Transform& GetTransform() { return scene.GetTransform(id_); }
    INLINE Geometry& GetGeometry() { return scene.GetGeometry(id_); }
    INLINE Script* GetScript() { return scene.GetScript(id_); }

    template<typename T> void AddScript();

    void AddMesh(const wchar_t *path);
    void AddMesh(Geometry::primitive type);

    [[nodiscard]] INLINE math::xvec3 Position() const { return scene.GetTransform(id_).Position(); }
    [[nodiscard]] INLINE math::xvec3 Rotation() const { return scene.GetTransform(id_).Rotation(); }
    [[nodiscard]] INLINE math::xvec3 Scale() const { return scene.GetTransform(id_).Scale(); }

private:
    u32 id_;
};

template<typename T>
void Entity::AddScript() {
    T* script = new T();
    scene.AddScript(script, id_);
}

}