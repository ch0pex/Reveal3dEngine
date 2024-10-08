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
#include "../scene.hpp"

#include <vector>
#include <set>

namespace reveal3d::core {

Transform::Transform(id_t id) : id_(id) { }

math::mat4& Transform::World() const {
    return Pool().Data().World(id_);
}

math::mat4& Transform::InvWorld() const {
    return Pool().Data().InvWorld(id_);
}

math::xvec3 Transform::Position() const {
    return Pool().Data().info.at(id::index(id_)).position;
}

math::xvec3 Transform::Scale() const {
    return Pool().Data().Info(id_).scale;
}

math::xvec3 Transform::Rotation() const {
    return math::VecToDegrees(Pool().Data().Info(id_).rotation);
}

math::xvec3 Transform::WorldPosition() const {
    math::mat4 worldMat = Pool().Data().World(id_);
    return worldMat.GetTranslation();
}

math::xvec3 Transform::WorldScale() const {
    return Pool().Data().World(id_).GetScale();
}

math::xvec3 Transform::WorldRotation() const {
    return Pool().Data().World(id_).GetRotation();
}

void Transform::SetPosition(math::xvec3 pos) const {
    Pool().Data().Info(id_).position = pos;
    SetDirty();
}

void Transform::SetScale(math::xvec3 scale) const {
    Pool().Data().Info(id_).scale = scale;
    SetDirty();
}

void Transform::SetRotation(math::xvec3 rot) const {
    Pool().Data().Info(id_).rotation = math::VecToRadians(rot);
    SetDirty();
}

void Transform::SetWorldPosition(const math::xvec3 pos) {
    Transform::Info& trans = Pool().Data().Info(id_);
    Pool().Data().World(id_) = math::Transpose(math::AffineTransformation(pos, trans.scale, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.position = math::Transpose(parent.Component<Transform>().InvWorld()) * pos;
    } else {
        trans.position = pos;
    }

    Pool().Data().InvWorld(id_) = math::Inverse(Pool().Data().World(id_));
    Pool().Dirties().at(id_) = 3;
    Pool().DirtyIds().insert(id_);
    UpdateChilds();
}

void Transform::SetWorldScale(const math::xvec3 size) {
    Transform::Info& trans = Pool().Data().Info(id_);
    Pool().Data().World(id_) = math::Transpose(math::AffineTransformation(trans.position, size, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.scale = parent.Component<Transform>().InvWorld() * size;
    } else {
        trans.scale = size;
    }
    Pool().Data().InvWorld(id_) = math::Inverse(Pool().Data().World(id_));
    Pool().Dirties().at(id::index(id_)) = 3;
    Pool().DirtyIds().insert(id::index(id_));
    UpdateChilds();
}

void Transform::SetWorldRotation(const math::xvec3 rot) {
    Transform::Info& trans = Pool().Data().Info(id_);
    World() = math::Transpose(math::AffineTransformation(trans.position, trans.scale, rot));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.rotation = parent.Component<Transform>().InvWorld() * rot;
    } else {
       trans.rotation = rot;
    }
    Pool().Data().InvWorld(id_) = math::Inverse(Pool().Data().World(id_));
    Pool().Dirties().at(id::index(id_)) = 3;
    Pool().DirtyIds().insert(id::index(id_));
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
    if (Pool().Dirties().at(id::index(id_)) != 4) return;

    Transform::Info& transform = Pool().Data().Info(id_);
    core::Scene::Node &currNode = scene.GetNode(id::index(id_));

    if (currNode.parent.IsAlive()) {
        const id_t parent_id = currNode.parent.Id();
        if (Pool().Dirties().at(id::index(id_)) < 4) {
            math::mat4 parentWorld = Pool().Data().World(parent_id);
            World() = parentWorld * CalcWorld(parent_id);
        } else {
            currNode.parent.Component<Transform>().Update();
            math::mat4 parentWorld = Pool().Data().World(parent_id);
            World() = parentWorld * CalcWorld(id_);
        }
    } else {
        World() = CalcWorld(id_);
    }
    InvWorld() = math::Transpose(math::Inverse(World()));
    --Pool().Dirties().at(id::index(id_));
}

void Transform::UnDirty() const {
    const id_t idx = id::index(id_);
    if (Pool().Dirties().at(idx) != 0) {
        --Pool().Dirties().at(idx);
    } else {
        Pool().Dirties().at(idx) = 0;
    }
}

void Transform::SetDirty() const {
    const id_t idx = id::index(Pool().GetMappedId(id_));
    if (Dirty() == 4)
        return;
    if (Dirty() == 0)
        Pool().DirtyIds().insert(id_);
    UpdateChilds();
    Pool().Dirties().at(idx) = 4;
}

u8 Transform::Dirty() const {
    return Pool().Dirties().at(id::index(id_));
}

Pool<Transform> &Transform::Pool() {
    return core::scene.ComponentPool<Transform>();
}



} // reveal3d::core namespace