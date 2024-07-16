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
#include "scene.hpp"

#include <vector>
#include <set>

namespace reveal3d::core::transform {

Transform::Transform(id_t id) : id_(id) {}

Transform::Transform(id_t id, math::mat4 &parentWorld) {
    id_t index { id::index(id) };

    if (transforms.size() > index) {
        transforms.at(index) = internal::Transform();
        world.at(index) = parentWorld;
        invWorld.at(index) = math::Mat4Identity();
        dirties.at(index) = 4;
        dirtyIds.insert(id_);
    }
    else {
        transforms.emplace_back();
        world.push_back(parentWorld);
        invWorld.at(index) = math::Mat4Identity();
        dirties.emplace_back(4);
        dirtyIds.insert(id_);
    }

}

Transform::Transform(id_t id, math::xvec3 pos) : id_(id) {
    id_t index { id::index(id) };
    internal::Transform transform;

    transform.position = pos;

    if (transforms.size() > index) {
        transforms[index] = transform;
    } else {
        transforms.emplace_back();
    }
}

math::mat4& Transform::World() const {
    return world.at(id::index(id_));
}

math::mat4& Transform::InvWorld() const {
    return invWorld.at(id::index(id_));
}

math::xvec3 Transform::Position() const {
    return transforms.at(id::index(id_)).position;
}

math::xvec3 Transform::Scale() const {
    return transforms.at(id::index(id_)).scale;
}

math::xvec3 Transform::Rotation() const {
    return math::VecToDegrees(transforms.at(id::index(id_)).rotation);
}

math::xvec3 Transform::WorldPosition() const {
    math::mat4 worldMat = world.at(id::index(id_));
    return worldMat.GetTranslation();
}

math::xvec3 Transform::WorldScale() const {
    return world.at(id::index(id_)).GetScale();
}

math::xvec3 Transform::WorldRotation() const {
    return world.at(id::index(id_)).GetRotation();
}

void Transform::SetPosition(math::xvec3 pos) const {
    id_t idx = id::index(id_);
    transforms.at(idx).position = pos;
    SetDirty();
}

void Transform::SetScale(math::xvec3 size) const {
    id_t idx = id::index(id_);
    transforms.at(idx).scale = size;
    SetDirty();
}

void Transform::SetRotation(math::xvec3 rot) const {
    id_t idx = id::index(id_);
    std::vector<internal::Transform> &transforms_ = transforms;
    transforms.at(idx).rotation = math::VecToRadians(rot);
    SetDirty();
}


void Transform::SetWorldPosition(const math::xvec3 pos) {
    id_t idx = id::index(id_);
    internal::Transform& trans = transforms.at(idx);
    world.at(idx) = math::Transpose(math::AffineTransformation(pos, trans.scale, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.position = math::Transpose(parent.Transform().InvWorld()) * pos;
    } else {
        trans.position = pos;
    }

    invWorld.at(idx) = math::Inverse(world.at(idx));
    dirties.at(idx) = 3;
    dirtyIds.insert(idx);
    UpdateChilds();
}

void Transform::SetWorldScale(const math::xvec3 size) {
    id_t idx = id::index(id_);
    internal::Transform& trans = transforms.at(idx);
    world.at(idx) = math::Transpose(math::AffineTransformation(trans.position, size, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.scale = parent.Transform().InvWorld() * size;
    } else {
        trans.scale = size;
    }
    invWorld.at(idx) = math::Inverse(world.at(idx));
    dirties.at(idx) = 3;
    dirtyIds.insert(idx);
    UpdateChilds();
}

void Transform::SetWorldRotation(const math::xvec3 rot) {
    id_t idx = id::index(id_);
    internal::Transform& trans = transforms.at(id::index(id_));
    World() = math::Transpose(math::AffineTransformation(trans.position, trans.scale, rot));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.rotation = parent.Transform().InvWorld() * rot;
    } else {
       trans.rotation = rot;
    }
    invWorld.at(idx) = math::Inverse(world.at(idx));
    dirties.at(idx) = 3;
    dirtyIds.insert(idx);
    UpdateChilds();
}

math::mat4 Transform::CalcWorld(id_t id){
    internal::Transform &transform = transforms.at(id::index(id));
    return math::Transpose(math::AffineTransformation(transform.position, transform.scale, transform.rotation));
}

void Transform::UpdateChilds() const {
    core::Scene::Node &node = scene.GetNode(id_);
    if (node.firstChild.IsAlive()) {
        core::Scene::Node &currNode = scene.GetNode(node.firstChild.Id());
        while(true) {
            currNode.entity.Transform().SetDirty();
            if (currNode.next.IsAlive() and currNode.next.Id() != node.next.Id()) {
                currNode = scene.GetNode(currNode.next.Id());
            } else { break; }
        }
    }
}

void Transform::UpdateWorld() {
    if (dirties.at(id::index(id_)) != 4) return;

    internal::Transform& transform = transforms[id::index(id_)];
    core::Scene::Node &currNode = scene.GetNode(id::index(id_));

    if (currNode.parent.IsAlive()) {
        id_t idx = id::index(currNode.parent.Id());
        if (dirties.at(idx) < 4) {
            math::mat4 parentWorld = world.at(idx);
            World() = parentWorld * CalcWorld(id_);
        } else {
            currNode.parent.Transform().UpdateWorld();
            math::mat4 parentWorld = world.at(idx);
            World() = parentWorld * CalcWorld(id_);
        }
    } else {
        World() = CalcWorld(id_);
    }
    InvWorld() = math::Transpose(math::Inverse(world.at(id::index(id_))));
    --dirties.at(id::index(id_));
}

void Transform::UnDirty() const {
    id_t idx = id::index(id_);
    if (dirties.at(idx) != 0) {
        --dirties.at(idx);
    } else {
        dirties.at(idx) = 0;
    }
}

void Transform::SetDirty() const {
    id_t idx = id::index(id_);
    if (dirties.at(idx) == 4)
        return;
    if (dirties.at(idx) == 0)
        dirtyIds.insert(id_);
    UpdateChilds();
    dirties.at(idx) = 4;
}

u8 Transform::Dirty() const {
    return dirties.at(id::index(id_));
}

void Scene::UpdateTransforms() {
    for (auto it = dirtyIds.begin(); it != dirtyIds.end();) {
        id_t idx = id::index(*it);
        GetEntity(idx).Transform().UpdateWorld();
        if (dirties.at(idx) == 0) {
            it = dirtyIds.erase(it);
        } else {
            ++it;
        }
    }
}

std::set<id_t>& Scene::DirtyTransforms() {
    return dirtyIds;
}

void reveal3d::core::TransformPool::AddComponent(id_t id) {
     id_t index { id::index(id) };

    if (transform_data_.size() > index) {
        transform_data_.at(index) = TransformData();
        world_.at(index) = math::Mat4Identity();
        invWorld_.at(index) = math::Mat4Identity();
        dirties_.at(index) = 4;
        dirtyIds_.insert(id_);
        transform_components_.emplace_back(id);
    }
    else {
        transforms.emplace_back();
        world.emplace_back(math::Mat4Identity());
        invWorld.emplace_back(math::Mat4Identity());
        dirties.emplace_back(4);
        dirtyIds.insert(id_);
    }   
}

void reveal3d::core::TransformPool::AddComponent(id_t id, Transform::InitInfo &init_info)
{

}

void reveal3d::core::TransformPool::AddChildComponent(id_t id, math::mat4 &parentWorld)
{

}

void reveal3d::core::TransformPool::RemoveComponent(id_t id)
{

}

}