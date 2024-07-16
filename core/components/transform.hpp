/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file transform.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "math/math.hpp"
#include "common/id.hpp"

#include <vector>
#include <set>

namespace reveal3d::core {

class Transform {
public:
    struct InitInfo {
        f32 pos[3]   { 0.f, 0.f, 0.f };
        f32 rot[3]   { 0.f, 0.f, 0.f };
        f32 scale[3] { 1.f, 1.f, 1.f };
    };

    Transform() = default;
    explicit Transform(id_t id);

    [[nodiscard]] math::mat4& World() const;
    [[nodiscard]] math::mat4& InvWorld() const;
    [[nodiscard]] math::xvec3 Position() const;
    [[nodiscard]] math::xvec3 Scale() const;
    [[nodiscard]] math::xvec3 Rotation() const;
    [[nodiscard]] math::xvec3 WorldPosition() const;
    [[nodiscard]] math::xvec3 WorldScale() const;
    [[nodiscard]] math::xvec3 WorldRotation() const;


    void SetPosition(math::xvec3 pos) const;
    void SetScale(math::xvec3 size) const;
    void SetRotation(math::xvec3 rot) const;
    void SetWorldPosition(math::xvec3 pos);
    void SetWorldScale(math::xvec3 size);
    void SetWorldRotation(math::xvec3 rot);
    void UpdateWorld();

    INLINE bool IsAlive() const { return id_ != id::invalid; }
    INLINE id_t Id() { return id_; }

    void UnDirty() const;
    void SetDirty() const;
    u8 Dirty() const;

private:
    static math::mat4 CalcWorld(id_t id);
    void UpdateChilds() const;

    id_t id_ { id::invalid };
};

class TransformPool {
public:
    void AddComponent(id_t id);
    void AddComponent(id_t id, Transform::InitInfo& init_info);
    void AddChildComponent(id_t id, math::mat4 &parentWorld);
    void RemoveComponent(id_t id);
    u32  Size();

    INLINE Transform At(id_t id)         { return Transform(id); }
    INLINE math::mat4& World(id_t id)    { return world_.at(id::index(id)); }
    INLINE math::mat4& InvWorld(id_t id) { return invWorld_.at(id::index(id)); }
    INLINE Transform Transform(id_t id)  { return transform_components_.at(id::index(id)); }

private:
    /************** Transform IDs ****************/
    std::vector<Transform>  transform_components_;

    /************* Transform Data ****************/
    struct TransformData {
        math::vec3 position;
        math::vec3 rotation;
        math::vec3 scale;
    }

    std::vector<math::mat4>     world_;
    std::vector<math::mat4>     invWorld_;
    std::vector<TransformData>  transform_data_;

    // Transforms that must be updated on GPU
    std::set<id_t>              dirtyIds_; 
    std::vector<u8>             dirties_;  
    
    friend class Transform;
};

}



