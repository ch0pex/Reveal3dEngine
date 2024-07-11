/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file scene.cpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Scene
 *
 * Scene IDs implementation
 */

#include "scene.hpp"

namespace reveal3d::core {

Scene scene;

namespace {

/*************** Entity IDs ***************/
std::vector<id_t>           generations;
std::deque<id_t>            freeIndices;

/************* Component IDs ***************/
std::vector<std::string>    names;
std::vector<Transform>      transforms;
std::vector<Geometry>       geometries;
std::vector<Script *>       scripts;
//std::vector<Light>          lights;

} // Anonymous namespace

Entity::Entity(std::string& name) {
    std::string id;

    GenerateId();
    id = std::to_string(id_);
    names.emplace_back(name + id);
}

Entity::Entity(u32 id) : id_ {id} { }

Entity::Entity(const wchar_t *path) {
    const std::string name = "New Entity ";

    GenerateId();
    names.emplace_back(name + std::to_string(id::index(id_)));
    transforms.emplace_back(id_);
    geometries.emplace_back(path);
    scripts.push_back(nullptr);
}

std::string &Entity::Name() const {
    return names.at(id::index(id_));
}

Transform& Entity::Transform() {
    return transforms.at(id::index(id_));
}

Geometry& Entity::Geometry() {
    return geometries.at(id::index(id_));
}

void Entity::GenerateId() {
    if (freeIndices.size() > id::minFree) {
        id_ = id::newGeneration(freeIndices.front());
        freeIndices.pop_front();
        ++generations[id::index(id_)];
    } else {
        id_ = generations.size();
        generations.push_back(0);
    }
}


void Entity::SetName(std::string_view name) {
    names.at(id::index(id_)) = std::string(name);
}
bool Entity::IsAlive() {
    if(id_ == id::invalid)
        return false;
    if (id::generation(id_) != generations.at(id::index(id_)))
        return false;
    return true;
}

Entity Scene::CreateEntity() {
    std::string name = "NewString";
    Entity entity(name);

    AddEntity(entity);

    return entity;
}

void Scene::AddEntity(Entity entity) {
    Node node {
        .entity = entity
    };

    if (lastNode != nullptr) {
       node.prev = lastNode->entity;
       lastNode->next = node.entity;
    }

    sceneGraph_.push_back(node);
    lastNode = &sceneGraph_.at(sceneGraph_.size() - 1);
}

Entity Scene::AddEntityFromObj(const wchar_t *path) {
    Entity entity(path);
    AddEntity(entity);
    return entity;
}

void Scene::AddChild(Entity child, Entity parent) {

    Node childNode {
            .entity = child,
            .parent = parent
    };

    Node& parentNode = sceneGraph_.at(id::index(parent.Id()));

    if (parentNode.firstChild.Id() == id::invalid) {
       parentNode.firstChild = child;
    } else {
        Node& currChild = sceneGraph_.at(id::index(parentNode.firstChild.Id()));
        while (currChild.next.Id() != parentNode.next.Id() and currChild.next.IsAlive()) {
           currChild = sceneGraph_.at(id::index(currChild.next.Id()));
        }
        currChild.next = childNode.entity;
        childNode.prev = currChild.entity;
        childNode.next = parentNode.next;
    }

    sceneGraph_.push_back(childNode);

}

void Scene::AddScript(Script *script, u32 id) {

}

void Scene::Init() {
//    for (u32 i = 0; i < scene.entities_; ++i) {
//        if (scripts_[i] != nullptr) {
//            Entity entity = GetEntity(i);
//            scripts_[i]->Begin(entity);
//        }
//    }
}

//Runs scripts
void Scene::Update(f32 dt) {
    UpdateTransforms();
//    UpdateGeometries();
//    UpdateScripts();

//    for (u32 i = 0; i < scene.entities_; ++i) {
//        if (scripts_[i] != nullptr) {
//            Entity entity = GetEntity(i);
//            scripts_[i]->Update(entity, dt);
//        }
//    }
}

Scene::~Scene() {
//    for(auto *script : scripts_) {
//        delete script;
//    }
}

std::vector<Transform> &Scene::Transforms() {
    return transforms;
}

std::vector<Geometry> &Scene::Geometries() {
    return geometries;
}

}
