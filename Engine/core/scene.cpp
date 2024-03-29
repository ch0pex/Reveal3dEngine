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

namespace reveal3d::core {


void Scene::AddPrimitive(Geometry::primitive primitiveType) {
    transforms_.emplace_back();
    geometries_.emplace_back(primitiveType);
    ++entities_;
}

void Scene::AddEntity(EntityInfo &entity) {
    transforms_.push_back(entity.transform);
    geometries_.push_back(entity.geometry);
}

void Scene::AddEntityFromObj(const wchar_t *path) {
    transforms_.emplace_back();
    geometries_.emplace_back(path);
    ++entities_;
}

}
