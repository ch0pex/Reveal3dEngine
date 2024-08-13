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

    if (!id_factory_.UseFree()) {
        transform_pool_.AddComponent(entity.Id());
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
        Node& lastchild = *parentnode.GetChildren().back();
        currchild.next = childNode.entity;
        childNode.prev = currChild.entity;
        childNode.next = parentNode.next;
    }

    sceneGraph_.push_back(childNode);
    
    return child;
}

void Scene::RemoveEntity(id_t id) {
    id_factory_.Remove(id);
    RemoveNode(id);
    transform_pool_.RemoveComponent(id);
    geometry_pool_.RemoveComponent(id);
}

void Scene::RemoveNode(id_t id) {
    if (!id::isValid(id)) return;
    //TODO remove childs
    sceneGraph_.erase(sceneGraph_.begin() + id::index(id));


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
    transform_pool_.Update();
    geometry_pool_.Update();
    script_pool_.Update();
    metadata_pool_.Update();
}

Scene::~Scene() {
//    for(auto *script : scripts_) {
//        delete script;
//    }
}

std::vector<Node*> Scene::Node::GetChildren() {
    Node &node = scene.GetNode(id_);
    std::vector<Node*> children;
    if (node.firstChild.IsAlive()) {
        Node &currNode = scene.GetNode(node.firstChild.Id());
        while(true) {
            children.push_back(&currNode);
            if (currNode.next.IsAlive() and currNode.next.Id() != node.next.Id()) {
                currNode = scene.GetNode(currNode.next.Id());
            } else {
                break;
            }
        }
    }
    return children;
}

}
