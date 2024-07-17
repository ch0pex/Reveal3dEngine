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
#include "render/light.hpp"

namespace reveal3d::core {

Scene scene;

Entity::Entity(u32 id) : id_ {id} { }

bool Entity::IsAlive() {
    return core::scene.IsEntityAlive(id_);
}


Entity Scene::NewEntity() {
    Entity entity(id_factory_.New());

    Node node {
        .entity = entity
    };

    if (lastNode_ != nullptr) {
       node.prev = lastNode_->entity;
       lastNode_->next = node.entity;
    }

    sceneGraph_.push_back(node);
    lastNode_ = &sceneGraph_.at(sceneGraph_.size() - 1);

    if (!id_factory_.FreeCap()) {
        transform_pool_.AddComponent();
        geometry_pool_.AddComponent();
    }

    return entity;
}

Entity Scene::NewChildEntity(Entity parent) {

    Entity child(id_factory_.New());

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
    
    return child;
}

void Scene::RemoveEntity(id_t id) {
    id_factory_.Remove(id);
    transform_pool_.RemoveComponent(id);
    geometry_pool_.RemoveComponent(id);
}

bool Scene::IsEntityAlive(id_t id) {
    return id_factory_.IsAlive(id);
}

void Scene::Init() {
    // Pooling Init
    // transform_pool_.Init();
    
    
//    for (u32 i = 0; i < scene.entities_; ++i) {
//        if (scripts_[i] != nullptr) {
//            Entity entity = GetEntity(i);
//            scripts_[i]->Begin(entity);
//        }
//    }
}

//Runs scripts
void Scene::Update(f32 dt) {
    // Pooling update
    transform_pool_.Update();
    // geometry_pool_.Update();
    // script_pool_.Update();
    // transform_pool_.Update();
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

}
