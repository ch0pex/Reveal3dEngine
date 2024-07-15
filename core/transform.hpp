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

struct Transform {

    math::vec3 position;
    math::vec3 rotation;
    math::vec3 scale;

};

namespace transform {

class Component {
    Component() = default;
    explicit Component(id_t id);
    Component(id_t id, math::mat4& parentWorld);
    Component(id_t id, math::xvec3 pos);
    //    Transform(id_t id, InitInfo& info);

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

class Pool {
public:
    void AddComponent(id_t id);
    void RemoveComponent(id_t id);
    void AddChildComponent(id_t id, math::mat4 &parentWorld);

    INLINE math::mat4& World(id_t id) { return world_.at(id::index(id)); }
    INLINE math::mat4& InvWorld(id_t id) { return invWorld_.at(id::index(id)); }
    INLINE Transform&  Data(id_t id) { return transform_data_.at(id::index(id)); }

private:
    std::vector<math::mat4> world_;
    std::vector<math::mat4> invWorld_;
    std::vector<Transform> transform_data_;

    std::vector<u8> dirties_;
    std::set<id_t> dirtyIds_;

    std::vector<Component> transform_components_;
};

}

}


