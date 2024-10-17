/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file transform.cpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "transform.hpp"
#include "core/scene.hpp"

namespace reveal3d::core {

Pool<Transform> &pool() {
    return core::scene.ComponentPool<Transform>();
}

Transform::Transform(id_t id) : id_(id) { }

math::mat4& Transform::World() const {
    return pool().Data().World(id_);
}

math::mat4& Transform::InvWorld() const {
    return pool().Data().InvWorld(id_);
}

math::xvec3 Transform::Position() const {
    return pool().Data().info.at(id::index(id_)).position;
}

math::xvec3 Transform::Scale() const {
    return pool().Data().Info(id_).scale;
}

math::xvec3 Transform::Rotation() const {
    return math::VecToDegrees(pool().Data().Info(id_).rotation);
}

math::xvec3 Transform::WorldPosition() const {
    math::mat4 worldMat = pool().Data().World(id_);
    return worldMat.GetTranslation();
}

math::xvec3 Transform::WorldScale() const {
    return pool().Data().World(id_).GetScale();
}

math::xvec3 Transform::WorldRotation() const {
    return pool().Data().World(id_).GetRotation();
}

void Transform::SetPosition(math::xvec3 pos) const {
    pool().Data().Info(id_).position = pos;
    SetDirty();
}

void Transform::SetScale(math::xvec3 scale) const {
    pool().Data().Info(id_).scale = scale;
    SetDirty();
}

void Transform::SetRotation(math::xvec3 rot) const {
    pool().Data().Info(id_).rotation = math::VecToRadians(rot);
    SetDirty();
}

void Transform::SetWorldPosition(const math::xvec3 pos) {
    Transform::Info& trans = pool().Data().Info(id_);
    pool().Data().World(id_) = math::Transpose(math::AffineTransformation(pos, trans.scale, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.position = math::Transpose(parent.Component<Transform>().InvWorld()) * pos;
    } else {
        trans.position = pos;
    }

    pool().Data().InvWorld(id_) = math::Inverse(pool().Data().World(id_));
    pool().Dirties().at(id_) = 3;
    pool().DirtyIds().insert(id_);
    UpdateChilds();
}

void Transform::SetWorldScale(const math::xvec3 size) {
    Transform::Info& trans = pool().Data().Info(id_);
    pool().Data().World(id_) = math::Transpose(math::AffineTransformation(trans.position, size, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.scale = parent.Component<Transform>().InvWorld() * size;
    } else {
        trans.scale = size;
    }
    pool().Data().InvWorld(id_) = math::Inverse(pool().Data().World(id_));
    pool().Dirties().at(id::index(id_)) = 3;
    pool().DirtyIds().insert(id_);
    UpdateChilds();
}

void Transform::SetWorldRotation(const math::xvec3 rot) {
    Transform::Info& trans = pool().Data().Info(id_);
    World() = math::Transpose(math::AffineTransformation(trans.position, trans.scale, rot));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.rotation = parent.Component<Transform>().InvWorld() * rot;
    } else {
       trans.rotation = rot;
    }
    pool().Data().InvWorld(id_) = math::Inverse(pool().Data().World(id_));
    pool().Dirties().at(id::index(id_)) = 3;
    pool().DirtyIds().insert(id_);
    UpdateChilds();
}

math::mat4 Transform::CalcWorld(id_t id){
    Transform::Info& trans = core::scene.ComponentPool<Transform>().Data().Info(id);
    return math::Transpose(math::AffineTransformation(trans.position, trans.scale, trans.rotation));
}

void Transform::UpdateChilds() const {
    core::Scene::Node &node = scene.GetNode(id_);
    auto children = node.GetChildren();
    for (auto child_id : children) {
        Entity child = Entity(child_id);
        child.Component<Transform>().SetDirty();
        child.Component<Transform>().UpdateChilds();
    }
}

void Transform::Update() const {
    if (pool().Dirties().at(id::index(id_)) != 4) return;

    Transform::Info& transform = pool().Data().Info(id_);
    core::Scene::Node &currNode = scene.GetNode(id::index(id_));

    if (currNode.parent.IsAlive()) {
        const id_t parent_id = currNode.parent.Id();
        if (pool().Dirties().at(id::index(id_)) < 4) {
            math::mat4 parentWorld = pool().Data().World(parent_id);
            World() = parentWorld * CalcWorld(parent_id);
        } else {
            currNode.parent.Component<Transform>().Update();
            math::mat4 parentWorld = pool().Data().World(parent_id);
            World() = parentWorld * CalcWorld(id_);
        }
    } else {
        World() = CalcWorld(id_);
    }
    InvWorld() = math::Transpose(math::Inverse(World()));
    --pool().Dirties().at(id::index(id_));
}

void Transform::UnDirty() const {
    const id_t idx = id::index(id_);
    if (pool().Dirties().at(idx) != 0) {
        --pool().Dirties().at(idx);
    } else {
        pool().Dirties().at(idx) = 0;
    }
}

void Transform::SetDirty() const {
    if (Dirty() == 4)
        return;
    if (Dirty() == 0)
        pool().DirtyIds().insert(id_);
    UpdateChilds();
    pool().Dirties().at(id::index(id_)) = 4;
}

u8 Transform::Dirty() const {
    return pool().Dirties().at(id::index(id_));
}




} // reveal3d::core namespace