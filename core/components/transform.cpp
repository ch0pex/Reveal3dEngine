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
    return Pool().World(id_);
}

math::mat4& Transform::InvWorld() const {
    return Pool().InvWorld(id_);
}

math::xvec3 Transform::Position() const {
    return Pool().transform_data_.at(id::index(id_)).position;
}

math::xvec3 Transform::Scale() const {
    return Pool().transform_data_.at(id::index(id_)).scale;
}

math::xvec3 Transform::Rotation() const {
    return math::VecToDegrees(Pool().transform_data_.at(id::index(id_)).rotation);
}

math::xvec3 Transform::WorldPosition() const {
    math::mat4 worldMat = Pool().World(id_);
    return worldMat.GetTranslation();
}

math::xvec3 Transform::WorldScale() const {
    return Pool().World(id_).GetScale();
}

math::xvec3 Transform::WorldRotation() const {
    return Pool().World(id_).GetRotation();
}

void Transform::SetPosition(math::xvec3 pos) const {
    Pool().transform_data_.at(id::index(id_)).position = pos;
    SetDirty();
}

void Transform::SetScale(math::xvec3 scale) const {
    Pool().transform_data_.at(id::index(id_)).scale = scale;
    SetDirty();
}

void Transform::SetRotation(math::xvec3 rot) const {
    Pool().transform_data_.at(id::index(id_)).rotation = math::VecToRadians(rot);
    SetDirty();
}

void Transform::SetWorldPosition(const math::xvec3 pos) {
    Transform::Data& trans = Pool().Data(id_);
    Pool().World(id_) = math::Transpose(math::AffineTransformation(pos, trans.scale, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.position = math::Transpose(parent.Component<Transform>().InvWorld()) * pos;
    } else {
        trans.position = pos;
    }

    Pool().InvWorld(id_) = math::Inverse(Pool().World(id_));
    Pool().dirties_.at(id_) = 3;
    Pool().dirtyIds_.insert(id_);
    UpdateChilds();
}

void Transform::SetWorldScale(const math::xvec3 size) {
    Transform::Data& trans = Pool().Data(id_);
    Pool().World(id_) = math::Transpose(math::AffineTransformation(trans.position, size, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.scale = parent.Component<Transform>().InvWorld() * size;
    } else {
        trans.scale = size;
    }
    Pool().InvWorld(id_) = math::Inverse(Pool().World(id_));
    Pool().dirties_.at(id::index(id_)) = 3;
    Pool().dirtyIds_.insert(id::index(id_));
    UpdateChilds();
}

void Transform::SetWorldRotation(const math::xvec3 rot) {
    Transform::Data& trans = Pool().Data(id_);
    World() = math::Transpose(math::AffineTransformation(trans.position, trans.scale, rot));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.rotation = parent.Component<Transform>().InvWorld() * rot;
    } else {
       trans.rotation = rot;
    }
    Pool().InvWorld(id_) = math::Inverse(Pool().World(id_));
    Pool().dirties_.at(id::index(id_)) = 3;
    Pool().dirtyIds_.insert(id::index(id_));
    UpdateChilds();
}

math::mat4 Transform::CalcWorld(id_t id){
    Transform::Data& trans = core::scene.ComponentPool<Transform>().Data(id);
    return math::Transpose(math::AffineTransformation(trans.position, trans.scale, trans.rotation));
}

void Transform::UpdateChilds() const {
    core::Scene::Node &node = scene.GetNode(id_);
    auto children = node.GetChildren();
    for (auto child : children) {
        child->entity.Component<Transform>().SetDirty();
        child->entity.Component<Transform>().UpdateChilds();
    }
}

void Transform::UpdateWorld() {
    if (Pool().dirties_.at(id::index(id_)) != 4) return;

    Transform::Data& transform = Pool().Data(id_);
    core::Scene::Node &currNode = scene.GetNode(id::index(id_));

    if (currNode.parent.IsAlive()) {
        const id_t parent_id = currNode.parent.Id();
        if (Pool().dirties_.at(id::index(id_)) < 4) {
            math::mat4 parentWorld = Pool().World(parent_id);
            World() = parentWorld * CalcWorld(parent_id);
        } else {
            currNode.parent.Component<Transform>().UpdateWorld();
            math::mat4 parentWorld = Pool().World(parent_id);
            World() = parentWorld * CalcWorld(id_);
        }
    } else {
        World() = CalcWorld(id_);
    }
    InvWorld() = math::Transpose(math::Inverse(World()));
    --Pool().dirties_.at(id::index(id_));
}

void Transform::UnDirty() const {
    const id_t idx = id::index(id_);
    if (Pool().dirties_.at(idx) != 0) {
        --Pool().dirties_.at(idx);
    } else {
        Pool().dirties_.at(idx) = 0;
    }
}

void Transform::SetDirty() const {
    const id_t idx = id::index(id_);
    if (Dirty() == 4)
        return;
    if (Dirty() == 0)
        Pool().dirtyIds_.insert(idx);
    UpdateChilds();
    Pool().dirties_.at(idx) = 4;
}

u8 Transform::Dirty() const {
    return Pool().dirties_.at(id::index(id_));
}

TransformPool &Transform::Pool() {
    return core::scene.ComponentPool<Transform>();
}

Transform TransformPool::AddComponent(id_t id) {
    return AddComponent(id, {});
}

Transform TransformPool::AddComponent(id_t entityId, Transform::Data &&initInfo) {
    id_t transformId{ id_factory_.New() };

    if (id_factory_.UseFree()) {
        Transform::Data& data = transform_data_.at(id::index(transformId));
        data = initInfo;
        world_.at(id::index(transformId)) = math::Transpose(math::AffineTransformation(data.position, data.scale, data.rotation));
        invWorld_.at(id::index(transformId)) = math::Inverse(world_.at(id::index(transformId)));
        dirties_.at(id::index(transformId)) = 4;
        dirtyIds_.insert(transformId);
    } else {
        transform_data_.push_back(std::move(initInfo));
        Transform::Data& data = transform_data_.at(Count() - 1);
        world_.emplace_back(math::Transpose(math::AffineTransformation(data.position, data.scale, data.rotation)));
        invWorld_.emplace_back(math::Inverse(world_.at(Count() - 1)));
        dirties_.emplace_back(4);
        dirtyIds_.insert(transformId);
    }

    Add(id::index(entityId), transformId);

    return components_.at(id::index(entityId));
}

Transform TransformPool::AddChildComponent(id_t entityId, math::mat4 &parentWorld) {
    id_t transformId{ id_factory_.New() };

    if (id_factory_.UseFree()) {
        transform_data_.at(id::index(transformId)) = Transform::Data();
        world_.at(id::index(transformId)) = math::Mat4Identity();
        invWorld_.at(id::index(transformId)) = math::Mat4Identity();
        dirties_.at(id::index(transformId)) = 4;
        dirtyIds_.insert(entityId);
    } else {
        transform_data_.emplace_back();
        world_.emplace_back(parentWorld);
        invWorld_.emplace_back(math::Inverse(parentWorld));
        dirties_.emplace_back(4);
        dirtyIds_.insert(transformId);
    }

    Add(id::index(entityId), transformId);
    return components_.at(id::index(entityId));
}

//NOTE all entities must have transform for now
void TransformPool::RemoveComponent(id_t id) {
    id_t idx { id::index(id) };
    if (id_factory_.IsAlive(id)) {
        transform_data_.unordered_remove(idx);
        world_.unordered_remove(idx);
        invWorld_.unordered_remove(idx);
        dirties_.unordered_remove(idx);
        if (dirtyIds_.find(id) != dirtyIds_.end()) {
            dirtyIds_.erase(id);
        }
        Remove(id);
    }
}

void TransformPool::Update() {
    for (auto it = dirtyIds_.begin(); it != dirtyIds_.end();) {
        id_t idx = id::index(*it);
        components_.at(idx).UpdateWorld();
        if (dirties_.at(idx) == 0) {
            it = dirtyIds_.erase(it);
        } else {
            ++it;
        }
    }
}

} // reveal3d::core namespace