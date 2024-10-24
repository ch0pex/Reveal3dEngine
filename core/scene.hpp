/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file scene.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief ECS
 *
 * Entity is_component system main header file.
 * In Reveal3D componentes just holds and ID that points to the real data.
 * Real data is compacted to avoid cache misses, this logic is handled by components pools
 *
 * *********************************************** Components pool *****************************************************
 * ************************** IDs **************************************************** data ****************************
 *                                                          *                                                          *
 * Enity IDs        |  00   | 01    |  02   |  03  | ...    * Enity IDs        |  00   | 01    |  02   |  03  | ...    *
 *                                                          *                                                          *
 * --------------------- Components IDs-------------------- * ------------------- Components data -------------------- *
 *                  ------- ------- ------- ------          *                  ------- ------- -------                 *
 * Transform IDs   |   00  |  01   |  FF   |  02  |  ...    * Transforms      |       |       |       |  ...           *
 *                  ------- ------- ------- ------          *                  ------- ------- -------                 *
 *                  ------- ------- ------- ------          *                  ------- ------- -------                 *
 * Geometry IDs    |  00   |  FF   |  01   |  02  |  ...    * Geometries      |       |       |       |  ...           *
 *                  ------- ------- ------- ------          *                  ------- ------- -------                 *
 *                  ------- ------- ------- ------          *                  ------- -------                         *
 * Script IDs      |  00   |  FF   |  FF   |  01   |  ...   * Scripts         |       |       |  ...                   *
 *                  ------- ------- ------- ------          *                  ------- -------                         *
 * *********************************************************************************************************************
 */

#pragma once

#include "common/common.hpp"
#include "components.hpp"
#include "render/light.hpp"

#include <deque>
#include <set>
#include <utility>
#include <vector>

namespace reveal3d::core {

class Entity {
public:
    Entity() : id_(id::invalid) {}
    explicit Entity(id_t id);

    template<is_component T>
    T component() const;
    template<is_component T>
    T addComponent();
    template<is_component T>
    T addComponent(T::InitInfo&& init_info);
    template<is_component T>
    void removeComponent();
    Entity addChild();

    [[nodiscard]] u32 id() const { return id_; }
    [[nodiscard]] bool isAlive() const;

private:
    id_t id_;
};


class Scene {
public:
    struct Node {
        [[nodiscard]] std::vector<id_t> getChildren() const;
        Entity entity;
        Entity parent;
        Entity first_child;
        Entity next;
        Entity prev;
    };

    Scene();
    ~Scene();

    Entity newEntity();
    Entity newChildEntity(Entity parent);
    Entity newChildEntity(id_t parent);
    Entity removeEntity(id_t id);
    bool isEntityAlive(id_t id);

    /******************************* Scene graph methods ******************************/

    [[nodiscard]] const std::vector<Scene::Node> &graph() const { return scene_graph_; }
    [[nodiscard]] u32 count() const { return scene_graph_.size() - free_nodes_.size(); }
    [[nodiscard]] Entity getEntity(id_t id) const { return scene_graph_.at(id::index(id)).entity; }
    Node &getNode(id_t id) { return scene_graph_.at(id::index(id)); }
    Node &root() { return scene_graph_.at(root_node_); }

    template<is_component T>
    Pool<T> &componentPool() noexcept;

    void init();
    void update(f32 dt);

private:
    void removeNode(id_t id);

    /************ Entities scene ***********/

    std::vector<Scene::Node> scene_graph_;
    u32 root_node_;
    u32 last_node_;
    std::deque<u32> free_nodes_;

    /*********** Components Pools  *************/

    Pool<core::Transform> transform_pool_;
    Pool<core::Geometry> geometry_pool_;
    Pool<core::Script> script_pool_;
    Pool<core::Metadata> metadata_pool_;

    //    LightPool                 light_pool_;
};

template<is_component T>
Pool<T> &Scene::componentPool() noexcept {
    if constexpr (std::is_same<T, Transform>()) {
        return (transform_pool_);
    }
    else if constexpr (std::is_same<T, Script>()) {
        return (script_pool_);
    }
    else if constexpr (std::is_same<T, Metadata>()) {
        return (metadata_pool_);
    }
    else {
        return (geometry_pool_);
    }
}

extern Scene scene;

template<is_component T>
T Entity::component() const {
    if (not isAlive()) {
        return T();
    }
    return scene.componentPool<T>().at(id_);
}

template<is_component T>
T Entity::addComponent() {
    if (not isAlive()) {
        return T();
    }
    return scene.componentPool<T>().addComponent(id_);
}

template<is_component T>
T Entity::addComponent(T::InitInfo &&init_info) {
    if (not isAlive()) {
        return T();
    }
    return scene.componentPool<T>().addComponent(id_, std::forward<T::InitInfo>(init_info));
}

template<is_component T>
void Entity::removeComponent() {
    if (isAlive()) {
        scene.componentPool<T>().removeComponent(id_);
    }
}

} // namespace reveal3d::core
