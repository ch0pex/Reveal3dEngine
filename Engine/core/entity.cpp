/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file entity.cpp
 * @version 1.0
 * @date 06/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "entity.hpp"

namespace reveal3d::core {

Entity::Entity() : id_(id::invalid) {}

Entity::Entity(u32 id) : id_ {id} {}

void Entity::AddMesh(const wchar_t *path) {
    // TODO: Check if has geometry, for now all have geometries
    scene.GetGeometry(id_).AddMesh(path);
}

void Entity::AddMesh(Geometry::primitive type) {
    scene.GetGeometry(id_).AddMesh(type);
}



}