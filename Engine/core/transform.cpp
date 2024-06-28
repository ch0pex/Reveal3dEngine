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


std::vector<internal::Transform> transforms;

} //Anonymous namesapce

Transform::Transform(id_t id) : id_(id) {
    id_t index { id::index(id) };

    if (transforms.size() > index)
        transforms[index] = internal::Transform();
    else
        transforms.emplace_back();

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

Transform::Transform(id_t id, Transform::InitInfo &info) : id_(id) {
    id_t index { id::index(id) };

    if (transforms.size() > index) {
        transforms[index].position = info.position;
        transforms[index].rotation = info.rotation;
        transforms[index].scale = info.scale;
    }
    else {
        transforms.emplace_back(info.position, info.rotation, info.scale);
    }
}

math::mat4 Transform::World() const {
    internal::Transform& transform = transforms[id::index(id_)];
    return math::Transpose(math::AffineTransformation(transform.position, transform.scale, transform.rotation));
}

math::xvec3 Transform::Position() const {
    return transforms[id::index(id_)].position;
}

math::xvec3 Transform::Scale() const {
    return transforms[id::index(id_)].scale;
}

math::xvec3 Transform::Rotation() const {
    return math::VecToDegrees(transforms[id::index(id_)].rotation);
}

u8 Transform::IsDirty() const {
    return dirty_;
}

void Transform::SetPosition(math::xvec3 pos) {
    transforms[id::index(id_)].position = pos;
    dirty_ = 3;
}

void Transform::SetScale(math::xvec3 size) {
    transforms[id::index(id_)].scale = size;
    dirty_ = 3;
}

void Transform::SetRotation(math::xvec3 rot) {
    transforms[id::index(id_)].rotation = math::VecToRadians(rot);
    dirty_ = 3;
}

}