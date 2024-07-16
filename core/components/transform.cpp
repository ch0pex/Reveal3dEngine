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

Transform::Transform(id_t id) : id_(id) {}

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
    return Pool().transform_data_.at(id::index(id_)).rotation;
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
    id_t idx = id::index(id_);
    Pool().transform_data_.at(idx).position = pos;
    SetDirty();
}

void Transform::SetScale(math::xvec3 size) const {
    id_t idx = id::index(id_);
    Pool().transform_data_.at(idx).scale = size;
    SetDirty();
}

void Transform::SetRotation(math::xvec3 rot) const {
    id_t idx = id::index(id_);
    Pool().transform_data_.at(idx).rotation = math::VecToRadians(rot);
    SetDirty();
}

void Transform::SetWorldPosition(const math::xvec3 pos) {
    id_t idx = id::index(id_);
    TransformPool::TransformData& trans = Pool().transform_data_.at(idx);
    Pool().World(idx) = math::Transpose(math::AffineTransformation(pos, trans.scale, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.position = math::Transpose(parent.Component<Transform>().InvWorld()) * pos;
    } else {
        trans.position = pos;
    }

    Pool().InvWorld(idx) = math::Inverse(Pool().World(idx));
    dirties.at(idx) = 3;
    dirtyIds.insert(idx);
    UpdateChilds();
}

void Transform::SetWorldScale(const math::xvec3 size) {
    id_t idx = id::index(id_);
    TransformPool::TransformData& trans = Pool().transform_data_.at(idx);
    Pool().World(idx) = math::Transpose(math::AffineTransformation(trans.position, size, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.scale = parent.Component<Transform>().InvWorld() * size;
    } else {
        trans.scale = size;
    }
    Pool().InvWorld(idx) = math::Inverse(Pool().World(idx));
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
    Pool().InvWorld(idx) = math::Inverse(Pool().World(idx));
    dirties.at(idx) = 3;
    dirtyIds.insert(idx);
    UpdateChilds();
}

math::mat4 Transform::CalcWorld(id_t id){
    TransformPool::TransformData& trans = core::scene.ComponentPool<Transform>().transform_data_.at(id::index(id));
    return math::Transpose(math::AffineTransformation(trans.position, trans.scale, trans.rotation));
}

void Transform::UpdateChilds() const {
    core::Scene::Node &node = scene.GetNode(id_);
    if (node.firstChild.IsAlive()) {
        core::Scene::Node &currNode = scene.GetNode(node.firstChild.Id());
        while(true) {
            currNode.entity.Component<Transform>().SetDirty();
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
    return Pool().Dirties(id::index(id_));
}

TransformPool &Transform::Pool() const {
    return core::scene.ComponentPool<Transform>();
}

//void Scene::UpdateTransforms() {
//    for (auto it = dirtyIds.begin(); it != dirtyIds.end();) {
//        id_t idx = id::index(*it);
//        GetEntity(idx).Transform().UpdateWorld();
//        if (dirties.at(idx) == 0) {
//            it = dirtyIds.erase(it);
//        } else {
//            ++it;
//        }
//    }
//}
//
//std::set<id_t>& Scene::DirtyTransforms() {
//    return dirtyIds;
//}

void reveal3d::core::TransformPool::AddComponent(id_t id) {
     id_t index { id::index(id) };

    if (transform_data_.size() > index) {
        transform_data_.at(index) = TransformData();
        world_.at(index) = math::Mat4Identity();
        invWorld_.at(index) = math::Mat4Identity();
        dirties_.at(index) = 4;
        dirtyIds_.insert(id);
        transform_components_.emplace_back(id);
    }
    else {
        transform_data_.emplace_back();
        world_.emplace_back(math::Mat4Identity());
        invWorld_.emplace_back(math::Mat4Identity());
        dirties_.emplace_back(4);
        dirtyIds_.insert(id);
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