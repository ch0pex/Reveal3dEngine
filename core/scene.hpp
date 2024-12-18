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
 * Entity component system main header file.
 * - In Reveal3D components just holds and ID that points to the real data stored in a pool.
 * - Components are proxy classes to access data stored in an aos pool
 * - Real data is compacted to avoid cache misses, this logic is handled by components pools.
 * - Each component has a proxy class (Component class it self) and a pool class with data
 *
 * ************************************************** Components pool **************************************************
 * ************************** IDs **************************************************** data ****************************
 *                                                          *                                                          *
 * Entity IDs        |  00   | 01    |  02   |  03  | ...   * Entity IDs        |  00   | 01    |  02   |  03  | ...   *
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

#include "pooling/geometry_pool.hpp"
#include "pooling/light_pool.hpp"
#include "pooling/metadata_pool.hpp"
#include "pooling/rigidbody_pool.hpp"
#include "pooling/script_pool.hpp"
#include "pooling/transform_pool.hpp"

#include "components/concepts.hpp"

#include <deque>
#include <vector>

#include "pooling/pool.hpp"

namespace reveal3d::core {

class Entity {
public:
  Entity() : id_(id::invalid) { }

  explicit Entity(id_t const id) : id_ {id} {};

  template<detail::is_component T>
  T component() const;

  template<detail::is_component T>
  T addComponent();

  template<detail::is_component T>
  T addComponent(typename T::init_info const& init_info);

  template<detail::is_component T>
  void removeComponent();

  Entity addChild() const;

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

  Scene() : root_node_(id::invalid), last_node_(id::invalid) { }

  ~Scene() = default;

  Entity newEntity() {
    id_t const id = id::maxFree < free_nodes_.size() ? id::new_generation(free_nodes_.front()) : scene_graph_.size();
    Entity const entity(id);

    Node node {.entity = entity};

    if (root_node_ == id::invalid) {
      root_node_ = id::index(entity.id());
    }

    if (id::is_valid(last_node_)) {
      node.prev                        = scene_graph_.at(last_node_).entity;
      scene_graph_.at(last_node_).next = node.entity;
    }

    addNode(node, entity.id());
    last_node_ = id::index(entity.id());

    return entity;
  }

  Entity newChildEntity(Entity const parent) {
    assert(parent.isAlive());

    id_t const id = (id::maxFree < free_nodes_.size()) ? id::new_generation(free_nodes_.front()) : scene_graph_.size();
    Entity const child(id);

    Node child_node {.entity = child, .parent = parent};

    Node& parent_node = getNode(parent.id());

    if (not parent_node.first_child.isAlive()) {
      parent_node.first_child = child;
    }
    else {
      Node& first_child       = getNode(parent_node.first_child.id());
      first_child.prev        = child_node.entity;
      child_node.next         = parent_node.first_child;
      parent_node.first_child = child_node.entity;
    }

    addNode(child_node, child.id());

    return child;
  }

  Entity newChildEntity(id_t const parent) { return newChildEntity(Entity(parent)); }

  /// TODO fix this function
  Entity removeEntity(id_t const id) {
    Entity entity {id};
    Entity nextOrPrev = {};
    if (isEntityAlive(id)) {
      if (isEntityAlive(getNode(id).next.id())) {
        nextOrPrev = getNode(id).next;
      }
      else if (isEntityAlive(getNode(id).prev.id())) {
        nextOrPrev = getNode(id).prev;
      }
      removeNode(id);
    }
    return {}; // TODO fix this can't return nextOrPrev when deleting nested nodes
  }

  bool isEntityAlive(id_t const id) {
    if (id == id::invalid) {
      return false;
    }
    if (id::index(id) >= scene_graph_.size())
      return false;
    return getNode(id).entity.id() != id::invalid;
  }

  /******************************* Scene graph methods ******************************/

  [[nodiscard]] std::vector<Node> const& graph() const { return scene_graph_; }

  [[nodiscard]] u32 count() const { return scene_graph_.size() - free_nodes_.size(); }

  [[nodiscard]] Entity getEntity(id_t const id) const { return scene_graph_.at(id::index(id)).entity; }

  Node& getNode(id_t const id) { return scene_graph_.at(id::index(id)); }

  Node& root() { return scene_graph_.at(root_node_); }

  /// @note Can't use concept here because T is not complete type
  template<typename T>
  decltype(auto) pool() noexcept {
    if constexpr (std::same_as<typename T::pool_type, transform::Pool>) {
      return (transform_pool_);
    }
    else if constexpr (std::same_as<typename T::pool_type, script::Pool>) {
      return (script_pool_);
    }
    else if constexpr (std::same_as<typename T::pool_type, metadata::Pool>) {
      return (metadata_pool_);
    }
    else if constexpr (std::same_as<typename T::pool_type, light::Pool>) {
      return (light_pool_);
    }
    else if constexpr (std::same_as<typename T::pool_type, rigidbody::Pool>) {
      return (rigidbody_pool_);
    }
    else {
      return (geometry_pool_);
    }
  }

  void init() { }

  /**
   * @brief Updating scene function
   *
   * This function updates every component pool each frame
   *
   * @param[in] dt Delta time
   * @return None
   *
   * @note Not all components need to be updated every frame
   */
  void update(f32 dt) {
    transform_pool_.update();
    geometry_pool_.update();
    assert(transform_pool_.count() == count());
    assert(metadata_pool_.count() == count());
  }

private:
  void removeNode(id_t const id) {
    if (!id::is_valid(id)) {
      return;
    }

    Node& node = getNode(id);
    free_nodes_.push_back(node.entity.id());
    transform_pool_.removeComponent(id);
    metadata_pool_.removeComponent(id);
    geometry_pool_.removeComponent(id);
    rigidbody_pool_.removeComponent(id);
    light_pool_.removeComponent(id);
    script_pool_.removeComponent(id);

    if (node.prev.isAlive()) {
      Node& prev_node = getNode(node.prev.id());
      prev_node.next  = node.next;
    }
    else if (not node.parent.isAlive()) {
      // Change root node
      root_node_ = node.next.isAlive() ? node.next.id() : id::invalid;
    }

    if (node.next.isAlive()) {
      Node& next_node = getNode(node.next.id());
      next_node.prev  = node.prev;
    }
    else if (not node.parent.isAlive()) {
      last_node_ = node.prev.isAlive() ? node.prev.id() : id::invalid;
    }

    if (node.first_child.isAlive()) {
      for (auto const children = node.getChildren(); auto const child: children) {
        removeNode(child);
      }
    }
    node.entity = {};
  }

  void addNode(Node const& node, id_t const id) {
    if (id::maxFree < free_nodes_.size()) {
      scene_graph_.at(id::index(id)) = node;
      free_nodes_.pop_front();
    }
    else {
      scene_graph_.push_back(node);
      geometry_pool_.addComponent();
      rigidbody_pool_.addComponent();
      light_pool_.addComponent();
      script_pool_.addComponent();
    }

    transform_pool_.addComponent(id);
    metadata_pool_.addComponent(id, {id, fmt::format("Entity_{}", id::index(id))});
  }


  /************ Scene entities ***********/

  std::vector<Node> scene_graph_;
  index_t root_node_;
  index_t last_node_;
  std::deque<index_t> free_nodes_;

  /*********** Components Pools  *************/

  GenericPool<transform::Pool> transform_pool_;
  GenericPool<geometry::Pool> geometry_pool_;
  GenericPool<script::Pool> script_pool_;
  GenericPool<metadata::Pool> metadata_pool_;
  GenericPool<rigidbody::Pool> rigidbody_pool_;
  GenericPool<light::Pool> light_pool_;
};

inline Scene scene;

template<detail::is_component T>
T Entity::component() const {
  if (not isAlive()) {
    return T();
  }
  return scene.pool<T>().at(id_);
}
template<detail::is_component T>
T Entity::addComponent() {
  return addComponent<T>({});
}

template<detail::is_component T>
T Entity::addComponent(typename T::init_info const& init_info) {
  if (not isAlive()) {
    return T();
  }
  return scene.pool<T>().addComponent(id_, init_info);
}

template<detail::is_component T>
void Entity::removeComponent() {
  if (isAlive()) {
    scene.pool<T>().removeComponent(id_);
  }
}

inline bool Entity::isAlive() const { return scene.isEntityAlive(id_); }

inline Entity Entity::addChild() const { return scene.newChildEntity(id_); }

inline std::vector<id_t> Scene::Node::getChildren() const {
  std::vector<id_t> children;
  if (first_child.isAlive()) {
    Node* curr_node = &scene.getNode(first_child.id());
    while (true) {
      children.push_back(curr_node->entity.id());
      if (curr_node->next.isAlive()) {
        curr_node = &scene.getNode(curr_node->next.id());
      }
      else {
        break;
      }
    }
  }
  return children;
}

} // namespace reveal3d::core
