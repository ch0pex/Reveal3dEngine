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

#include <deque>
#include <vector>

#include "common/tuple.hpp"
#include "core/pooling/pools_map.hpp"
#include "pooling/pool.hpp"

namespace reveal3d::core {

class Scene {
public:
  struct Node {
    id_t entity {id::invalid};
    id_t parent {id::invalid};
    id_t first_child {id::invalid};
    id_t next {id::invalid};
    id_t prev {id::invalid};
  };

  Scene() : root_node_(id::invalid), last_node_(id::invalid) { }

  ~Scene() = default;

  id_t newEntity() {
    id_t const entity =
        id::maxFree < free_nodes_.size() ? id::new_generation(free_nodes_.front()) : scene_graph_.size();

    Node node {.entity = entity};

    if (root_node_ == id::invalid) {
      root_node_ = id::index(entity);
    }

    if (id::is_valid(last_node_)) {
      node.prev                        = scene_graph_.at(last_node_).entity;
      scene_graph_.at(last_node_).next = node.entity;
    }

    addNode(node, entity);
    last_node_ = id::index(entity);

    return entity;
  }

  id_t newChildEntity(id_t const parent) {
    assert(isAlive(parent));

    id_t const child =
        (id::maxFree < free_nodes_.size()) ? id::new_generation(free_nodes_.front()) : scene_graph_.size();

    Node child_node {.entity = child, .parent = parent};

    Node& parent_node = scene_graph_.at(id::index(parent));

    if (not isAlive(parent_node.first_child)) {
      parent_node.first_child = child;
    }
    else {
      Node& first_child       = scene_graph_.at(id::index(parent_node.first_child));
      first_child.prev        = child_node.entity;
      child_node.next         = parent_node.first_child;
      parent_node.first_child = child_node.entity;
    }

    addNode(child_node, child);

    return child;
  }

  /// TODO fix this function
  id_t removeEntity(id_t const id) {
    id_t nextOrPrev = {};
    if (isAlive(id)) {
      if (isAlive(getNode(id).next)) {
        nextOrPrev = getNode(id).next;
      }
      else if (isAlive(getNode(id).prev)) {
        nextOrPrev = getNode(id).prev;
      }
      removeNode(id);
    }
    return {}; // TODO fix this can't return nextOrPrev when deleting nested nodes
  }

  bool isAlive(id_t const id) {
    if (id == id::invalid) {
      return false;
    }
    if (id::index(id) >= scene_graph_.size())
      return false;
    return getNode(id).entity != id::invalid;
  }

  /******************************* Scene graph methods ******************************/

  [[nodiscard]] std::vector<Node> const& graph() const { return scene_graph_; }

  [[nodiscard]] u32 count() const { return scene_graph_.size() - free_nodes_.size(); }

  [[nodiscard]] id_t entity(index_t const idx) const {
    return scene_graph_.at(id::index(idx)).entity;
  } // @note carefull with id::index

  Node const& getNode(id_t const id) { return scene_graph_.at(id::index(id)); }

  Node& root() { return scene_graph_.at(root_node_); }

  /// @note Can't use concept here because T is not complete type
  template<typename T>
  decltype(auto) pool() noexcept {
    return (pools_.get<typename T::pool_type>());
  }

  void init() { }

  /**
   * @brief Updating scene function
   *
   * This function updates every component pool each frame
   *
   * @param[in] dt Delta time
   * @return None
   */
  void update(f32 dt) {
    tuple::for_each(pools_.data, [&](auto&& pool) { pool.update(this); });
  }

private:
  using pool_map_type = PoolMap< //
      transform::Pool, //
      geometry::Pool, //
      script::Pool, //
      metadata::Pool, //
      rigidbody::Pool, //
      light::Pool //
      >;

  std::vector<id_t> getChildren(Node const& node) {
    std::vector<id_t> children;
    if (isAlive(node.first_child)) {
      Node const* curr_node = &getNode(node.first_child);
      while (true) {
        children.push_back(curr_node->entity);
        if (isAlive(curr_node->next)) {
          curr_node = &getNode(curr_node->next);
        }
        else {
          break;
        }
      }
    }
    return children;
  }

  void removeNode(id_t const id) {
    if (not id::is_valid(id)) {
      return;
    }

    Node& node = scene_graph_.at(id::index(id));
    free_nodes_.push_back(node.entity);

    tuple::for_each(pools_.data, [&](auto&& pool) { pool.removeComponent(id); });

    if (isAlive(node.prev)) {
      Node& prev_node = scene_graph_.at(id::index(node.prev));
      prev_node.next  = node.next;
    }
    else if (not isAlive(node.parent)) {
      // Change root node
      root_node_ = isAlive(node.next) ? node.next : id::invalid;
    }

    if (isAlive(node.next)) {
      Node& next_node = scene_graph_.at(id::index(node.next));
      next_node.prev  = node.prev;
    }
    else if (not isAlive(node.parent)) {
      last_node_ = isAlive(node.prev) ? node.prev : id::invalid;
    }

    if (isAlive(node.first_child)) {
      for (auto const children = getChildren(node); auto const child: children) {
        removeNode(child);
      }
    }
    node.entity = {id::invalid};
  }

  void addNode(Node const& node, id_t const id) {
    if (id::maxFree < free_nodes_.size()) {
      scene_graph_.at(id::index(id)) = node;
      free_nodes_.pop_front();
    }
    else {
      scene_graph_.push_back(node);
      tuple::for_each(pools_.data, [&](auto&& pool) { pool.addComponent(); });
    }

    pools_.get<transform::Pool>().addComponent(id, {});
    pools_.get<metadata::Pool>().addComponent(id, {id, fmt::format("Entity_{}", id::index(id))});
  }


  /************ Scene entities ***********/

  std::vector<Node> scene_graph_;
  index_t root_node_;
  index_t last_node_;
  std::deque<index_t> free_nodes_;

  /*********** Components Pools  *************/

  pool_map_type pools_;
};


} // namespace reveal3d::core
