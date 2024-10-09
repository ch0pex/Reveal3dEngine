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

class Transform {
public:
    struct Info {
        math::xvec3 position    { 0.f, 0.f, 0.f };
        math::xvec3 rotation    { 0.f, 0.f, 0.f };
        math::xvec3 scale       { 1.f, 1.f, 1.f };
    };

    /************* Transform Data ****************/
    struct Data {
        math::mat4& World(id_t id)       { return world.at(id::index(id)); }
        math::mat4& InvWorld(id_t id)    { return invWorld.at(id::index(id)); }
        Transform::Info& Info(id_t id)   { return info.at(id::index(id)); }
        const u32 Count()                { return info.size(); }

        utl::vector<math::mat4>         world;
        utl::vector<math::mat4>         invWorld;
        utl::vector<Transform::Info>    info;
    };

    using InitInfo = Info;

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
    void Update() const;

    [[nodiscard]] INLINE bool IsAlive() const { return id_ != id::invalid; }
    [[nodiscard]] INLINE id_t Id() const { return id_; }

    [[nodiscard]] u8 Dirty() const;
    void UnDirty() const;
    void SetDirty() const;


private:
    [[nodiscard]] static Pool<Transform>& Pool();
    static math::mat4 CalcWorld(id_t id);
    void UpdateChilds() const;

    id_t id_ { id::invalid };
};

template<>
inline Transform Pool<Transform>::AddComponent(id_t entityId, Transform::InitInfo &&initInfo) {
    id_t transformId {id_factory_.New()};

    data_.info.push_back(std::move(initInfo));
    Transform::Info& data = data_.info.at(Count() - 1);
    data_.world.emplace_back(math::Transpose(math::AffineTransformation(data.position, data.scale, data.rotation)));
    data_.invWorld.emplace_back(math::Inverse(data_.world.at(Count() - 1)));
    Dirties().emplace_back(4);
    DirtyIds().insert(entityId);

    Add(id::index(entityId), transformId);

    return components_.at(id::index(entityId));
}

template<>
inline Transform Pool<Transform>::AddComponent(id_t id) {
    return AddComponent(id, {});
}

template<>
inline void Pool<Transform>::RemoveComponent(id_t id) {
    id_t transform_id { components_.at(id::index(id)).Id() };
    if (id_factory_.IsAlive(id)) {
        data_.info.unordered_remove(id::index(transform_id));
        data_.world.unordered_remove(id::index(transform_id));
        data_.invWorld.unordered_remove(id::index(transform_id));
        Dirties().at(id::index(id)) = 0;
        if (DirtyIds().find(transform_id) != DirtyIds().end()) {
            DirtyIds().erase(transform_id);
        }
        Remove(transform_id);
    }
}

} // reveal3d::core namespace



