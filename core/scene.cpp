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

bool Entity::isAlive() const {
    return core::scene.isEntityAlive(id_);
}

Entity Entity::addChild() {
    return core::scene.newChildEntity(id_);
}

Scene::Scene() : root_node_(id::invalid), last_node_(id::invalid) {}


Entity Scene::newEntity() {
    const id_t id = id::maxFree < free_nodes_.size() ? id::newGeneration(free_nodes_.front()) : scene_graph_.size();
    Entity entity(id);

    Node node {
        .entity = entity
    };

    if (root_node_ == id::invalid) {
        root_node_ = id::index(entity.id());
    }

    if (id::isValid(last_node_)) {
       node.prev = scene_graph_.at(last_node_).entity;
       scene_graph_.at(last_node_).next = node.entity;
    }

    if (id::maxFree < free_nodes_.size()) {
        scene_graph_.at(id::index(entity.id())) = node;
        free_nodes_.pop_front();
    } else {
        scene_graph_.push_back(node);
        geometry_pool_.addComponent();
    }

    transform_pool_.addComponent(entity.id());
    metadata_pool_.addComponent(entity.id());

    last_node_ = id::index(entity.id());
//    nodeIndex_.push_back(id::index(entity.id()));

    return entity;
}

Entity Scene::newChildEntity(id_t parent) { return newChildEntity(Entity(parent)); }

Entity Scene::newChildEntity(Entity parent) {
    assert(parent.isAlive());

    const id_t id = id::maxFree < free_nodes_.size() ? id::newGeneration(free_nodes_.front()) : scene_graph_.size();
    Entity child(id);

    Node child_node{
            .entity = child,
            .parent = parent
    };

    Node&parent_node = getNode(parent.id());

    if (not parent_node.first_child.isAlive()) {
        parent_node.first_child = child;
    } else {
        Node&first_child = getNode(parent_node.first_child.id());
        first_child.prev = child_node.entity;
        child_node.next = parent_node.first_child;
        parent_node.first_child = child_node.entity;
    }

    if (id::maxFree < free_nodes_.size()) {
        scene_graph_.at(id::index(child.id())) = child_node;
        free_nodes_.pop_front();
    } else {
        scene_graph_.push_back(child_node);
        geometry_pool_.addComponent();
    }

    transform_pool_.addComponent(child.id());
    metadata_pool_.addComponent(child.id());
//    nodeIndex_.push_back(id::index(child.id()));

    return child;
}

Entity Scene::removeEntity(id_t id) {
    Entity entity { id };
    Entity nextOrPrev = {};
    if (isEntityAlive(id)) {
        if (isEntityAlive(getNode(id).next.id())) nextOrPrev = getNode(id).next;
        else if (isEntityAlive(getNode(id).prev.id())) nextOrPrev = getNode(id).prev;
        removeNode(id);
    }
    return {}; //TODO fix this can't return nextOrPrev when deleting nested nodes
}

void Scene::removeNode(id_t id) {
    if (!id::isValid(id)) return;
    Node& node = getNode(id);
    free_nodes_.push_back(node.entity.id());
    transform_pool_.removeComponent(id);
    metadata_pool_.removeComponent(id);
    geometry_pool_.removeComponent(id);


    if (node.prev.isAlive()) {
        Node& prev_node = getNode(node.prev.id());
        prev_node.next = node.next;
    } else if (not node.parent.isAlive()){
       //Change root node
       root_node_ = node.next.isAlive() ? node.next.id() : id::invalid;
    }

    if (node.next.isAlive()) {
        Node& next_node = getNode(node.next.id());
        next_node.prev = node.prev;
    } else if (not node.parent.isAlive()){
        last_node_ = node.prev.isAlive() ? node.prev.id() : id::invalid;
    }

    if (node.first_child.isAlive()) {
        auto children = node.getChildren();
        for (auto child : children) {
            removeNode(child);
        }
    }
    node.entity = {};
}

bool Scene::isEntityAlive(id_t id) {
    if (id == id::invalid) return false;
    if (id::index(id) >= scene_graph_.size()) return false;
    return getNode(id).entity.id() != id::invalid;
}

void Scene::init() {
    // Pooling init
    // transformPool_.init();
    
    
//    for (u32 i = 0; i < scene.entities_; ++i) {
//        if (scripts_[i] != nullptr) {
//            Entity entity = getEntity(i);
//            scripts_[i]->begin(entity);
//        }
//    }
}

//Runs scripts
void Scene::update(f32 dt) {
    transform_pool_.update();
    geometry_pool_.update();
    script_pool_.update();
    metadata_pool_.update();
    assert(transform_pool_.count() == count());
    assert(metadata_pool_.count() == count());
}

Scene::~Scene() {
//    for(auto *script : scripts_) {
//        delete script;
//    }
}


std::vector<id_t> Scene::Node::getChildren() const {
    std::vector<id_t> children;
    if (first_child.isAlive()) {
        Node *curr_node = &scene.getNode(first_child.id());
        while(true) {
            children.push_back(curr_node->entity.id());
            if (curr_node->next.isAlive()) {
                curr_node = &scene.getNode(curr_node->next.id());
            } else {
                break;
            }
        }
    }
    return children;
}

}
