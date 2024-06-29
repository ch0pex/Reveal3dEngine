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
std::vector<id_t> dirtyIds;

} //Anonymous namesapce

Transform::Transform(id_t id) : id_(id) {
    id_t index { id::index(id) };

    if (transforms.size() > index) {
        transforms[index] = internal::Transform();
        world.at(index) = math::Mat4Identity();
        dirties.at(index) = 4;
        dirtyIds.push_back(id_);
    }
    else {
        transforms.emplace_back();
        world.emplace_back(math::Mat4Identity());
        dirties.emplace_back(4);
        dirtyIds.emplace_back(id_);
    }

}

Transform::Transform(id_t id, math::mat4 &parentWorld) {
    id_t index { id::index(id) };

    if (transforms.size() > index) {
        transforms.at(index) = internal::Transform();
        world.at(index) = parentWorld;
        dirties.at(index) = 4;
        dirtyIds.push_back(id_);
    }
    else {
        transforms.emplace_back();
        world.push_back(parentWorld);
        dirties.emplace_back(4);
        dirtyIds.push_back(id_);
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

std::vector<id_t> Transform::UpdateWorld(math::mat4 parentWorld) {
    if (dirties.at(id::index(id_)) != 4) return {};
    --dirties.at(id::index(id_));

    internal::Transform& transform = transforms[id::index(id_)];
    std::vector<id_t> newDirties;

    world.at(id::index(id_)) = parentWorld * math::Transpose(math::AffineTransformation(transform.position, transform.scale, transform.rotation));
    core::Scene::Node &currNode = scene.GetNode(id::index(id_));

    if (currNode.firstChild.IsAlive()) {
        core::Scene::Node &childNode = scene.GetNode(id::index(currNode.firstChild.Id()));
        while (true) {
            newDirties.push_back(childNode.entity.Id());
            dirties.at(id::index(childNode.entity.Id())) = 4;
            childNode.entity.Transform().UpdateWorld(World());
            if (childNode.next.Id() != currNode.next.Id() and childNode.next.IsAlive())
                childNode = core::scene.GetNode(id::index(childNode.next.Id()));
            else
                break;
        }
    }
    return newDirties;
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
    if (dirties.at(idx) == 0)
        dirtyIds.push_back(id_);
    dirties.at(idx) = 4;
}

void Scene::UpdateTransforms() {
    std::vector<id_t> newDirties;
    do {
        for (auto it = dirtyIds.begin(); it != dirtyIds.end();) {
            id_t idx = id::index(*it);
            Entity parent = GetNode(idx).parent;
            std::vector<id_t> entities;
            if (parent.IsAlive()) {
                entities = GetEntity(idx).Transform().UpdateWorld(parent.Transform().World());
            } else {
                entities = GetEntity(idx).Transform().UpdateWorld();
            }
            newDirties.insert(newDirties.end(), entities.begin(), entities.end());
            if (dirties.at(idx) == 0) {
                it = dirtyIds.erase(it);
            } else {
                ++it;
            }
        }
        dirtyIds.insert(dirtyIds.end(), newDirties.begin(), newDirties.end());
        newDirties.clear();
    } while(newDirties.size());

}

std::vector<id_t> &Scene::DirtyTransforms() {
    return dirtyIds;
}


}