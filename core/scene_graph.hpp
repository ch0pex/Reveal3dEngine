/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file scene_graph.hpp
 * @version 1.0
 * @date 14/02/2025
 * @brief Scene graph class
 *
 */

#pragma once


namespace reveal3d::core {

class SceneGraph {
public:
  struct Node {
    id_t entity {id::invalid};
    id_t parent {id::invalid};
    id_t first_child {id::invalid};
    id_t next {id::invalid};
    id_t prev {id::invalid};
  };

  id_t newId() { return id::maxFree < free_nodes_.size() ? id::new_generation(free_nodes_.front()) : nodes.size(); }

  bool isAlive(id_t const id) {
    if (id == id::invalid) {
      return false;
    }
    if (id::index(id) >= nodes.size())
      return false;
    return at(id::index(id)).entity != id::invalid;
  }

  template<typename Self>
  auto&& at(this Self&& self, id_t const id) {
    return std::forward<Self>(self).nodes.at(id::index(id));
  }

  auto begin() { return nodes.begin(); }

  auto end() { return nodes.end(); }

  auto begin() const { return nodes.begin(); }

  auto end() const { return nodes.end(); }

  auto count() const { return nodes.size() - free_nodes_.size(); }

  auto size() const { return nodes.size(); }

  void removeNode(id_t const id) {
    if (not id::is_valid(id)) {
      return;
    }

    Node& node = nodes.at(id::index(id));
    free_nodes_.push_back(node.entity);

    // tuple::for_each(pools_.data, [&](auto&& pool) { pool.removeComponent(id); });

    if (isAlive(node.prev)) {
      Node& prev_node = nodes.at(id::index(node.prev));
      prev_node.next  = node.next;
    }
    else if (not isAlive(node.parent)) {
      // Change root node
      root_node_ = isAlive(node.next) ? node.next : id::invalid;
    }

    if (isAlive(node.next)) {
      Node& next_node = nodes.at(id::index(node.next));
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

  void addNode(Node& node, id_t const id) {
    if (root_node_ == id::invalid) {
      root_node_ = id::index(id);
    }

    if (id::is_valid(last_node_) and not id::is_valid(node.parent)) {
      node.prev                 = nodes.at(last_node_).entity;
      nodes.at(last_node_).next = node.entity;
    }

    if (id::maxFree < free_nodes_.size()) {
      nodes.at(id::index(id)) = node;
      free_nodes_.pop_front();
    }
    else {
      nodes.push_back(node);
    }
    last_node_ = id::index(id);
  }

  std::vector<id_t> getChildren(Node const& node) {
    std::vector<id_t> children;
    if (isAlive(node.first_child)) {
      Node const* curr_node = &at(id::index(node.first_child));
      while (true) {
        children.push_back(curr_node->entity);
        if (isAlive(curr_node->next)) {
          curr_node = &at(id::index(curr_node->next));
        }
        else {
          break;
        }
      }
    }
    return children;
  }

  auto freeNodes() const { return free_nodes_.size(); }

private:
  std::vector<Node> nodes {};
  index_t root_node_ {id::invalid};
  index_t last_node_ {id::invalid};
  std::deque<index_t> free_nodes_ {};
};

} // namespace reveal3d::core
