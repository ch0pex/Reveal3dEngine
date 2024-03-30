/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file scene.cpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "scene.hpp"
#include "entity.hpp"

namespace reveal3d::core {


Entity Scene::AddPrimitive(Geometry::primitive primitiveType) {
    transforms_.emplace_back();
    geometries_.emplace_back(primitiveType);
    return Entity(entities_++, *this);

}

Entity Scene::AddEntity(EntityInfo &entity) {
    transforms_.push_back(entity.transform);
    geometries_.push_back(entity.geometry);
    return Entity(entities_++, *this);

}

Entity Scene::AddEntityFromObj(const wchar_t *path) {
    transforms_.emplace_back();
    geometries_.emplace_back(path);
    return Entity(entities_++, *this);

}

}
