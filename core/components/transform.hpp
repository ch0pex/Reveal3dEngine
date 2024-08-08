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

#include "pool.hpp"

#include <vector>
#include <set>

namespace reveal3d::core {

class TransformPool;

class Transform {
public:
    struct Data {
        math::xvec3 position    { 0.f, 0.f, 0.f };
        math::xvec3 rotation    { 0.f, 0.f, 0.f };
        math::xvec3 scale       { 1.f, 1.f, 1.f };
    };

    using PoolType = TransformPool&;
    using InitInfo = Data;

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

    [[nodiscard]] INLINE bool IsAlive() const { return id_ != id::invalid; }
    [[nodiscard]] INLINE id_t Id() const { return id_; }

    [[nodiscard]] u8 Dirty() const;
    void UnDirty() const;
    void SetDirty() const;


private:
    [[nodiscard]] static TransformPool& Pool();
    static math::mat4 CalcWorld(id_t id);
    void UpdateChilds() const;

    id_t id_ { id::invalid };
};

class TransformPool : public Pool<Transform> {
public:
    Transform AddComponent(id_t id);
    Transform AddComponent(id_t id, Transform::Data&& initInfo);
    Transform AddChildComponent(id_t id, math::mat4 &parentWorld);
    void RemoveComponent(id_t id) override;
    void Update() override;

    INLINE u32  Count()  override           { return transform_data_.size(); }
    INLINE std::set<id_t>&  DirtyElements() { return dirtyIds_; }

private:
    friend class Transform;
    INLINE math::mat4& World(id_t id)       { return world_.at(id::index(id)); }
    INLINE math::mat4& InvWorld(id_t id)    { return invWorld_.at(id::index(id)); }
    INLINE Transform::Data& Data(id_t id)   { return transform_data_.at(id::index(id)); }

    /************* Transform Data ****************/

    std::vector<math::mat4>       world_;
    std::vector<math::mat4>       invWorld_;
    std::vector<Transform::Data>  transform_data_;

    // Transforms that must be updated on GPU
    std::set<id_t>                dirtyIds_; 
    std::vector<u8>               dirties_;  
};

} // reveal3d::core namespace



