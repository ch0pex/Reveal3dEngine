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

Scene scene;

Entity Scene::AddPrimitive(Geometry::primitive primitiveType) {
    const std::string name = "New entity ";
    const std::string id = std::to_string(scene.NumEntities());
    names_.emplace_back(name + id);
    transforms_.emplace_back();
    geometries_.emplace_back(primitiveType);
    scripts_.push_back(nullptr);
    return Entity(entities_++);

}

Entity Scene::AddEntity(EntityInfo &entity) {
    const std::string name = "New entity ";
    const std::string id = std::to_string(scene.NumEntities());
    names_.emplace_back(name + id);
    transforms_.push_back(entity.transform);
    geometries_.push_back(entity.geometry);
    scripts_.push_back(nullptr);
    return Entity(entities_++);
}

Entity Scene::AddEntityFromObj(const wchar_t *path) {
    const std::string name = "New entity ";
    const std::string id = std::to_string(scene.NumEntities());
    names_.emplace_back(name + id);
    transforms_.emplace_back();
    geometries_.emplace_back(path);
    scripts_.push_back(nullptr);
    return Entity(entities_++);
}

void Scene::AddScript(Script *script, u32 id) {
    scripts_[id] = script;
}

void Scene::Init() {
    for (u32 i = 0; i < scene.entities_; ++i) {
        if (scripts_[i] != nullptr) {
            Entity entity = GetEntity(i);
            scripts_[i]->Begin(entity);
        }
    }
}

//Runs scripts
void Scene::Update(f32 dt) {
    for (u32 i = 0; i < scene.entities_; ++i) {
        if (scripts_[i] != nullptr) {
            Entity entity = GetEntity(i);
            scripts_[i]->Update(entity, dt);
        }
    }
}

Entity Scene::AddEntity(math::vec3 pos) {
    transforms_.push_back(Transform(pos));
    geometries_.push_back(Geometry());
    return Entity(entities_++);
}

Entity Scene::GetEntity(u32 id) {
    return Entity(id);
}

Scene::~Scene() {
    for(auto *script : scripts_) {
        delete script;
    }
}

}
