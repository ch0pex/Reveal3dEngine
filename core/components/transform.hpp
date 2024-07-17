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

class TransformPool;

class Transform {
public:
    struct Data {
        math::xvec3 pos   { 0.f, 0.f, 0.f };
        math::xvec3 rot   { 0.f, 0.f, 0.f };
        math::xvec3 scale { 1.f, 1.f, 1.f };
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
    void SetWorldScale(math::xvec3 scale);
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
    TransformPool& Pool() const;

    id_t id_ { id::invalid };
};

class TransformPool {
public:
    void AddComponent(id_t id, Transform::Data&& initInfo = {});
    void AddChildComponent(id_t id, math::mat4 &parentWorld);
    void RemoveComponent(id_t id);
    void Update();

    INLINE u32  Count()                     { return transform_data_.size(); }
    INLINE Transform At(id_t id)            { return transform_component_.at(id::index(id)); }
    INLINE std::set<id_t>&  DirtyElements() { return dirtyIds_; }

    std::vector<Transform>::iterator begin();
    std::vector<Transform>::iterator end();

private:
    friend class Transform;

    INLINE math::mat4& World(id_t id)    { return world_.at(id::index(id)); }
    INLINE math::mat4& InvWorld(id_t id) { return invWorld_.at(id::index(id)); }
    INLINE Transform::Data& Data(id_t id){ return transform_data_.at(id::index(id)); }

    /************** Transform IDs ****************/
    id::Factory                   id_factory_;
    std::vector<Transform>        transform_components_;

    /************* Transform Data ****************/

    std::vector<math::mat4>       world_;
    std::vector<math::mat4>       invWorld_;
    std::vector<Transform::Data>  transform_data_;

    // Transforms that must be updated on GPU
    std::set<id_t>                dirtyIds_; 
    std::vector<u8>               dirties_;  
};

} // reveal3d::core namespace



