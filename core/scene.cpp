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

Scene::Scene() : lastNode_(id::invalid) {}


Entity Scene::NewEntity() {
    Entity entity(idFactory_.New(sceneGraph_.size()));

    Node node {
        .entity = entity
    };

    if (id::isValid(lastNode_)) {
       node.prev = sceneGraph_.at(lastNode_).entity;
       sceneGraph_.at(lastNode_).next = node.entity;
    }

    sceneGraph_.push_back(node);
    lastNode_ = sceneGraph_.size() - 1U;

    if (!idFactory_.UseFree()) {
        transformPool_.AddComponent(entity.Id());
        metadataPool_.AddComponent(entity.Id());
        geometryPool_.AddComponent();
    }

    return entity;
}

Entity Scene::NewChildEntity(id_t parent) { return NewChildEntity(Entity(parent)); }

Entity Scene::NewChildEntity(Entity parent) {

    Entity child(idFactory_.New(sceneGraph_.size()));

    Node childNode {
            .entity = child,
            .parent = parent
    };

    Node& parentNode = sceneGraph_.at(id::index(parent.Id()));

    if (not parentNode.firstChild.IsAlive()) {
       parentNode.firstChild = child;
    } else {
        Node& firstChild = GetNode(parentNode.firstChild.Id());
        firstChild.prev = childNode.entity;
        childNode.next = parentNode.firstChild;
        parentNode.firstChild = childNode.entity;
//        childNode.next = parentNode.next;
    }

    sceneGraph_.push_back(childNode);

    if (!idFactory_.UseFree()) {
        transformPool_.AddComponent(child.Id());
        metadataPool_.AddComponent(child.Id());
        geometryPool_.AddComponent();
    }

    return child;
}

void Scene::RemoveEntity(id_t id) {
    if (idFactory_.IsAlive(id)) {
        idFactory_.Remove(id);
        RemoveNode(id);
        transformPool_.RemoveComponent(id);
        geometryPool_.RemoveComponent(id);
    }
}

void Scene::RemoveNode(id_t id) {
    if (!id::isValid(id)) return;
    //TODO remove childs
    sceneGraph_.erase(sceneGraph_.begin() + id::index(id));


}

bool Scene::IsEntityAlive(id_t id) {
    return idFactory_.IsAlive(id);
}

void Scene::Init() {
    // Pooling Init
    // transformPool_.Init();
    
    
//    for (u32 i = 0; i < scene.entities_; ++i) {
//        if (scripts_[i] != nullptr) {
//            Entity entity = GetEntity(i);
//            scripts_[i]->Begin(entity);
//        }
//    }
}

//Runs scripts
void Scene::Update(f32 dt) {
    transformPool_.Update();
    geometryPool_.Update();
    scriptPool_.Update();
    metadataPool_.Update();
}

Scene::~Scene() {
//    for(auto *script : scripts_) {
//        delete script;
//    }
}


std::vector<id_t> Scene::Node::GetChildren() {
    std::vector<id_t> children;
    if (firstChild.IsAlive()) {
        Node &currNode = scene.GetNode(firstChild.Id());
        while(true) {
            children.push_back(currNode.entity.Id());
            if (currNode.next.IsAlive()) {
                currNode = scene.GetNode(currNode.next.Id());
            } else {
                break;
            }
        }
    }
    return children;
}

}
