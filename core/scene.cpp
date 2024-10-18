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

bool Entity::IsAlive() const {
    return core::scene.IsEntityAlive(id_);
}

Entity Entity::AddChild() {
    return core::scene.NewChildEntity(id_);
}

Scene::Scene() : rootNode_(id::invalid), lastNode_(id::invalid) {}


Entity Scene::NewEntity() {
    const id_t id = id::maxFree < freeNodes_.size() ? id::newGeneration(freeNodes_.front()) : sceneGraph_.size();
    Entity entity(id);

    Node node {
        .entity = entity
    };

    if (rootNode_ == id::invalid) {
       rootNode_ = id::index(entity.Id());
    }

    if (id::isValid(lastNode_)) {
       node.prev = sceneGraph_.at(lastNode_).entity;
       sceneGraph_.at(lastNode_).next = node.entity;
    }

    if (id::maxFree < freeNodes_.size()) {
        sceneGraph_.at(id::index(entity.Id())) = node;
        freeNodes_.pop_front();
    } else {
        sceneGraph_.push_back(node);
        geometryPool_.AddComponent();
    }

    transformPool_.AddComponent(entity.Id());
    metadataPool_.AddComponent(entity.Id());

    lastNode_ = id::index(entity.Id());
//    nodeIndex_.push_back(id::index(entity.Id()));

    return entity;
}

Entity Scene::NewChildEntity(id_t parent) { return NewChildEntity(Entity(parent)); }

Entity Scene::NewChildEntity(Entity parent) {
    assert(parent.IsAlive());

    const id_t id = id::maxFree < freeNodes_.size() ? id::newGeneration(freeNodes_.front()) : sceneGraph_.size();
    Entity child(id);

    Node childNode {
            .entity = child,
            .parent = parent
    };

    Node& parentNode = GetNode(parent.Id());

    if (not parentNode.firstChild.IsAlive()) {
       parentNode.firstChild = child;
    } else {
        Node& firstChild = GetNode(parentNode.firstChild.Id());
        firstChild.prev = childNode.entity;
        childNode.next = parentNode.firstChild;
        parentNode.firstChild = childNode.entity;
    }

    if (id::maxFree < freeNodes_.size()) {
        sceneGraph_.at(id::index(child.Id())) = childNode;
        freeNodes_.pop_front();
    } else {
        sceneGraph_.push_back(childNode);
        geometryPool_.AddComponent();
    }

    transformPool_.AddComponent(child.Id());
    metadataPool_.AddComponent(child.Id());
//    nodeIndex_.push_back(id::index(child.Id()));

    return child;
}

Entity Scene::RemoveEntity(id_t id) {
    Entity entity { id };
    Entity nextOrPrev = {};
    if (IsEntityAlive(id)) {
        if (IsEntityAlive(GetNode(id).next.Id())) nextOrPrev = GetNode(id).next;
        else if (IsEntityAlive(GetNode(id).prev.Id())) nextOrPrev = GetNode(id).prev;
        RemoveNode(id);
    }
    return {}; //TODO fix this can't return nextOrPrev when deleting nested nodes
}

void Scene::RemoveNode(id_t id) {
    if (!id::isValid(id)) return;
    Node& node = GetNode(id);
    freeNodes_.push_back(node.entity.Id());
    transformPool_.RemoveComponent(id);
    metadataPool_.RemoveComponent(id);
    geometryPool_.RemoveComponent(id);


    if (node.prev.IsAlive()) {
        Node& prevNode = GetNode(node.prev.Id());
        prevNode.next = node.next;
    } else if (not node.parent.IsAlive()){
       //Change root node
       rootNode_ = node.next.IsAlive() ? node.next.Id() : id::invalid;
    }

    if (node.next.IsAlive()) {
        Node& nextNode = GetNode(node.next.Id());
        nextNode.prev = node.prev;
    } else if (not node.parent.IsAlive()){
        lastNode_ = node.prev.IsAlive() ? node.prev.Id() : id::invalid;
    }

    if (node.firstChild.IsAlive()) {
        auto children = node.GetChildren();
        for (auto child : children) {
            RemoveNode(child);
        }
    }
    node.entity = {};
}

bool Scene::IsEntityAlive(id_t id) {
    if (id == id::invalid) return false;
    if (id::index(id) >= sceneGraph_.size()) return false;
    return GetNode(id).entity.Id() != id::invalid;
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
    assert(transformPool_.Count() == Count());
    assert(metadataPool_.Count() == Count());
}

Scene::~Scene() {
//    for(auto *script : scripts_) {
//        delete script;
//    }
}


std::vector<id_t> Scene::Node::GetChildren() const {
    std::vector<id_t> children;
    if (firstChild.IsAlive()) {
        Node *currNode = &scene.GetNode(firstChild.Id());
        while(true) {
            children.push_back(currNode->entity.Id());
            if (currNode->next.IsAlive()) {
                currNode = &scene.GetNode(currNode->next.Id());
            } else {
                break;
            }
        }
    }
    return children;
}

}
