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

namespace reveal3d::core {


namespace {
namespace internal {

struct Transform {
    math::xvec3 position { 0.0f, 0.0f, 0.0f };
    math::xvec3 rotation { 0.0f, 0.0f, 0.0f };
    math::xvec3 scale    { 1.0f, 1.0f, 1.0f };
};

}


std::vector<math::mat4> world;
std::vector<math::mat4> invWorld;
std::vector<internal::Transform> transforms;

std::vector<u8> dirties;
std::set<id_t> dirtyIds;

} //Anonymous namesapce

Transform::Transform(id_t id) : id_(id) {
    id_t index { id::index(id) };

    if (transforms.size() > index) {
        transforms[index] = internal::Transform();
        world.at(index) = math::Mat4Identity();
        dirties.at(index) = 4;
        dirtyIds.insert(id_);
    }
    else {
        transforms.emplace_back();
        world.emplace_back(math::Mat4Identity());
        dirties.emplace_back(4);
        dirtyIds.insert(id_);
    }

}

Transform::Transform(id_t id, math::mat4 &parentWorld) {
    id_t index { id::index(id) };

    if (transforms.size() > index) {
        transforms.at(index) = internal::Transform();
        world.at(index) = parentWorld;
        dirties.at(index) = 4;
        dirtyIds.insert(id_);
    }
    else {
        transforms.emplace_back();
        world.push_back(parentWorld);
        dirties.emplace_back(4);
        dirtyIds.insert(id_);
//        invWorld.push_back();
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

math::mat4 Transform::World() const {
    return world.at(id::index(id_));
}

math::mat4 Transform::InvWorld() const {
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

void Transform::SetPosition(math::xvec3 pos) {
    id_t idx = id::index(id_);
    transforms.at(idx).position = pos;
    SetDirty();
}

void Transform::SetScale(math::xvec3 size) {
    id_t idx = id::index(id_);
    transforms.at(idx).scale = size;
    SetDirty();
}

void Transform::SetRotation(math::xvec3 rot) {
    id_t idx = id::index(id_);
    transforms.at(idx).rotation = math::VecToRadians(rot);
    SetDirty();
}

void Transform::UpdateWorld() {
    if (dirties.at(id::index(id_)) != 4) return;

    internal::Transform& transform = transforms[id::index(id_)];
    core::Scene::Node &currNode = scene.GetNode(id::index(id_));

    if (currNode.parent.IsAlive()) {
        id_t idx = id::index(currNode.parent.Id());
        if (dirties.at(idx) < 4) {
            math::mat4 parentWorld = world.at(idx);
            world.at(id::index(id_)) = parentWorld * math::Transpose(math::AffineTransformation(transform.position, transform.scale, transform.rotation));
        } else {
            currNode.parent.Transform().UpdateWorld();
            math::mat4 parentWorld = world.at(idx);
            world.at(id::index(id_)) = parentWorld * math::Transpose(math::AffineTransformation(transform.position, transform.scale, transform.rotation));
        }
    } else {
        world.at(id::index(id_)) = math::Transpose(math::AffineTransformation(transform.position, transform.scale, transform.rotation));
    }

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


}