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

Component::Component(id_t id) : id_(id) {
    id_t index { id::index(id) };

    if (transforms.size() > index) {
        transforms.at(index) = internal::Component();
        world.at(index) = math::Mat4Identity();
        invWorld.at(index) = math::Mat4Identity();
        dirties.at(index) = 4;
        dirtyIds.insert(id_);
    }
    else {
        transforms.emplace_back();
        world.emplace_back(math::Mat4Identity());
        invWorld.emplace_back(math::Mat4Identity());
        dirties.emplace_back(4);
        dirtyIds.insert(id_);
    }

}

Component::Component(id_t id, math::mat4 &parentWorld) {
    id_t index { id::index(id) };

    if (transforms.size() > index) {
        transforms.at(index) = internal::Component();
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

Component::Component(id_t id, math::xvec3 pos) : id_(id) {
    id_t index { id::index(id) };
    internal::Component transform;

    transform.position = pos;

    if (transforms.size() > index) {
        transforms[index] = transform;
    } else {
        transforms.emplace_back();
    }
}

math::mat4& Component::World() const {
    return world.at(id::index(id_));
}

math::mat4& Component::InvWorld() const {
    return invWorld.at(id::index(id_));
}

math::xvec3 Component::Position() const {
    return transforms.at(id::index(id_)).position;
}

math::xvec3 Component::Scale() const {
    return transforms.at(id::index(id_)).scale;
}

math::xvec3 Component::Rotation() const {
    return math::VecToDegrees(transforms.at(id::index(id_)).rotation);
}

math::xvec3 Component::WorldPosition() const {
    math::mat4 worldMat = world.at(id::index(id_));
    return worldMat.GetTranslation();
}

math::xvec3 Component::WorldScale() const {
    return world.at(id::index(id_)).GetScale();
}

math::xvec3 Component::WorldRotation() const {
    return world.at(id::index(id_)).GetRotation();
}

void Component::SetPosition(math::xvec3 pos) const {
    id_t idx = id::index(id_);
    transforms.at(idx).position = pos;
    SetDirty();
}

void Component::SetScale(math::xvec3 size) const {
    id_t idx = id::index(id_);
    transforms.at(idx).scale = size;
    SetDirty();
}

void Component::SetRotation(math::xvec3 rot) const {
    id_t idx = id::index(id_);
    std::vector<internal::Component> &transforms_ = transforms;
    transforms.at(idx).rotation = math::VecToRadians(rot);
    SetDirty();
}


void Component::SetWorldPosition(const math::xvec3 pos) {
    id_t idx = id::index(id_);
    internal::Component& trans = transforms.at(idx);
    world.at(idx) = math::Transpose(math::AffineComponentation(pos, trans.scale, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.position = math::Transpose(parent.Component().InvWorld()) * pos;
    } else {
        trans.position = pos;
    }

    invWorld.at(idx) = math::Inverse(world.at(idx));
    dirties.at(idx) = 3;
    dirtyIds.insert(idx);
    UpdateChilds();
}

void Component::SetWorldScale(const math::xvec3 size) {
    id_t idx = id::index(id_);
    internal::Component& trans = transforms.at(idx);
    world.at(idx) = math::Transpose(math::AffineComponentation(trans.position, size, trans.rotation));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.scale = parent.Component().InvWorld() * size;
    } else {
        trans.scale = size;
    }
    invWorld.at(idx) = math::Inverse(world.at(idx));
    dirties.at(idx) = 3;
    dirtyIds.insert(idx);
    UpdateChilds();
}

void Component::SetWorldRotation(const math::xvec3 rot) {
    id_t idx = id::index(id_);
    internal::Component& trans = transforms.at(id::index(id_));
    World() = math::Transpose(math::AffineComponentation(trans.position, trans.scale, rot));
    core::Entity parent = scene.GetNode(id_).parent;
    if (parent.IsAlive()) {
        trans.rotation = parent.Component().InvWorld() * rot;
    } else {
       trans.rotation = rot;
    }
    invWorld.at(idx) = math::Inverse(world.at(idx));
    dirties.at(idx) = 3;
    dirtyIds.insert(idx);
    UpdateChilds();
}

math::mat4 Component::CalcWorld(id_t id){
    internal::Component &transform = transforms.at(id::index(id));
    return math::Transpose(math::AffineComponentation(transform.position, transform.scale, transform.rotation));
}

void Component::UpdateChilds() const {
    core::Scene::Node &node = scene.GetNode(id_);
    if (node.firstChild.IsAlive()) {
        core::Scene::Node &currNode = scene.GetNode(node.firstChild.Id());
        while(true) {
            currNode.entity.Component().SetDirty();
            if (currNode.next.IsAlive() and currNode.next.Id() != node.next.Id()) {
                currNode = scene.GetNode(currNode.next.Id());
            } else { break; }
        }
    }
}

void Component::UpdateWorld() {
    if (dirties.at(id::index(id_)) != 4) return;

    internal::Component& transform = transforms[id::index(id_)];
    core::Scene::Node &currNode = scene.GetNode(id::index(id_));

    if (currNode.parent.IsAlive()) {
        id_t idx = id::index(currNode.parent.Id());
        if (dirties.at(idx) < 4) {
            math::mat4 parentWorld = world.at(idx);
            World() = parentWorld * CalcWorld(id_);
        } else {
            currNode.parent.Component().UpdateWorld();
            math::mat4 parentWorld = world.at(idx);
            World() = parentWorld * CalcWorld(id_);
        }
    } else {
        World() = CalcWorld(id_);
    }
    InvWorld() = math::Transpose(math::Inverse(world.at(id::index(id_))));
    --dirties.at(id::index(id_));
}

void Component::UnDirty() const {
    id_t idx = id::index(id_);
    if (dirties.at(idx) != 0) {
        --dirties.at(idx);
    } else {
        dirties.at(idx) = 0;
    }
}

void Component::SetDirty() const {
    id_t idx = id::index(id_);
    if (dirties.at(idx) == 4)
        return;
    if (dirties.at(idx) == 0)
        dirtyIds.insert(id_);
    UpdateChilds();
    dirties.at(idx) = 4;
}

u8 Component::Dirty() const {
    return dirties.at(id::index(id_));
}

void Scene::UpdateComponents() {
    for (auto it = dirtyIds.begin(); it != dirtyIds.end();) {
        id_t idx = id::index(*it);
        GetEntity(idx).Component().UpdateWorld();
        if (dirties.at(idx) == 0) {
            it = dirtyIds.erase(it);
        } else {
            ++it;
        }
    }
}

std::set<id_t>& Scene::DirtyComponents() {
    return dirtyIds;
}


}