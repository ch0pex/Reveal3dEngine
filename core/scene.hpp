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
#include "scene_graph.hpp"

namespace reveal3d::core {

class Scene {
public:
  using Node = SceneGraph::Node;

  Scene() { }

  ~Scene() = default;

  id_t newEntity() {
    id_t const entity = scene_graph_.newId();
    Node node {.entity = entity};


    scene_graph_.addNode(node, entity);
    if (id::maxFree >= scene_graph_.freeNodes()) {
      tuple::for_each(pools_.data, [&](auto&& pool) { pool.addComponent(); });
    }

    pools_.get<transform::Pool>().addComponent(entity, {});
    pools_.get<metadata::Pool>().addComponent(entity, {entity, fmt::format("Entity_{}", id::index(entity))});

    return entity;
  }

  id_t newChildEntity(id_t const parent) {
    assert(isAlive(parent));

    id_t const child = scene_graph_.newId();

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

    scene_graph_.addNode(child_node, child);

    pools_.get<transform::Pool>().addComponent(child, {});
    pools_.get<metadata::Pool>().addComponent(child, {child, fmt::format("Entity_{}", id::index(child))});

    return child;
  }

  /// TODO fix this function
  id_t removeEntity(id_t const id) {
    id_t nextOrPrev = {};
    id_t index      = id::index(id);
    if (isAlive(id)) {
      if (isAlive(scene_graph_.at(index).next)) {
        nextOrPrev = scene_graph_.at(index).next;
      }
      else if (isAlive(scene_graph_.at(index).prev)) {
        nextOrPrev = scene_graph_.at(index).prev;
      }
      tuple::for_each(pools_.data, [&](auto&& pool) { pool.removeComponent(id); });
      scene_graph_.removeNode(id);
    }
    return {}; // TODO fix this can't return nextOrPrev when deleting nested nodes
  }

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

  bool isAlive(id_t const id) { return scene_graph_.isAlive(id); }

  [[nodiscard]] auto node(id_t const id) const { return scene_graph_.at(id::index(id)); }

  [[nodiscard]] auto graph() const { return std::span {scene_graph_}; }

  [[nodiscard]] id_t entity(id_t const index) const { return scene_graph_.at(index).entity; }

  auto count() const { return scene_graph_.count(); }

private:
  using pool_map_type = PoolMap<
      transform::Pool, geometry::Pool, script::Pool, //
      metadata::Pool, rigidbody::Pool, light::Pool //
      >;

  /************ Scene entities ***********/
  SceneGraph scene_graph_ {};

  /*********** Components Pools  *************/
  pool_map_type pools_;
};


} // namespace reveal3d::core
