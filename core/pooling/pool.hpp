/************************************************************************
 * copyright (c) 2024 alvaro cabrera barrio
 * this code is licensed under mit license (see license.txt for details)
 ************************************************************************/
/**
 * @file pool.hpp
 * @version 1.0
 * @date 26/03/2024
 * @brief GenericPool virtual class
 *
 */

#pragma once

#include "common/common.hpp"
#include "core/pooling/concepts.hpp"

#include <queue>
#include <set>
#include <vector>

#include "config/config.hpp"

namespace reveal3d::core {

namespace detail {

template<typename InGPU>
class GpuSynchronize { };

template<>
class GpuSynchronize<std::true_type> {
public:
  id_t popNew() {
    if (new_components_.empty()) {
      return id::invalid;
    }
    auto const id = new_components_.front();
    new_components_.pop();
    return id;
  }
  id_t popRemoved() {
    if (deleted_components_.empty()) {
      return id::invalid;
    }
    auto const id = deleted_components_.front();
    deleted_components_.pop();
    return id;
  }
  std::set<id_t>& dirtyElements() { return dirty_ids_; }
  std::set<id_t>& dirtyIds() { return dirty_ids_; }
  utl::vector<u8>& dirties() { return dirties_; }

protected:
  std::queue<id_t> new_components_;
  std::queue<id_t> deleted_components_;
  std::set<id_t> dirty_ids_;
  utl::vector<u8> dirties_;
};

} // namespace detail

template<typename T>
class GenericPool : public T, public detail::GpuSynchronize<typename T::gpu_stored> {
public:
  using value_type = id_t;
  using iterator   = std::vector<value_type>::iterator;

  id_t addComponent() {
    components_ids_.emplace_back(id::invalid);
    if constexpr (stored_in_gpu<T>) {
      this->dirties_.emplace_back(config::render.graphics.buffer_count + 1);
    }
    return components_ids_.at(components_ids_.size() - 1);
  }

  id_t addComponent(id_t const entity_id) { return addComponent(entity_id, {}); }

  id_t addComponent(id_t const entity_id, typename T::init_info const& init_info) {
    id_t const component_id {id_factory_.newId(id::index(entity_id))};
    index_t const component_index {id::index(component_id)};

    this->addData(init_info);
    if constexpr (stored_in_gpu<T>) {
      if (this->dirties().size() > component_index) { // When adding new component it should be marked as dirty
        this->dirties().at(component_index) = config::render.graphics.buffer_count + 1;
      }
      else {
        this->dirties().emplace_back(config::render.graphics.buffer_count + 1);
      }
      this->dirtyIds().insert(component_id);

      if constexpr (std::same_as<T, geometry::Pool>) { // TODO change this for a concept
        this->new_components_.push(component_id);
      }
    }

    addId(id::index(entity_id), component_id);
    assert(id::index(component_id) < this->countData());

    return components_ids_.at(id::index(entity_id));
  }

  void removeComponent(id_t const entity_id) {
    if (id_t const component_id {components_ids_.at(id::index(entity_id))}; id_factory_.isAlive(component_id)) {
      this->removeData(component_id); // Removes data
      if constexpr (std::same_as<T, geometry::Pool>) { // TODO change this for a concept
        this->deleted_components_.push(component_id);
      }
      removeId(component_id); // Removes id
    }
  }

  /// @note for now this must be specialized in every component header file
  void update();

  u32 count() { return this->countData(); }

  id_t at(id_t const id) { return components_ids_.at(id::index(id)); }

  iterator begin() { return components_ids_.begin(); }

  iterator end() { return components_ids_.end(); }

  constexpr u32 getMappedId(id_t const component_id) { return id_factory_.mapped(id::index(component_id)); }

private:
  void addId(index_t const index, id_t id) {
    if (index >= components_ids_.size()) {
      components_ids_.emplace_back(id);
    }
    else {
      components_ids_.at(index) = id;
    }
  }

  void removeId(id_t const id) {
    id_t const last = components_ids_.at(id_factory_.back());
    index_t const component_index {id_factory_.mapped(id)};
    id_t const new_id = id::new_generation(id, id::generation(last));
    if (last != id) {
      components_ids_.at(id::index(getMappedId(last))) = new_id;
    }
    components_ids_.at(component_index) = id::invalid;
    id_factory_.remove(id);
    if constexpr (stored_in_gpu<T>) {
      if (last != id) {
        this->dirties_.at(id::index(id)) = config::render.graphics.buffer_count;
        this->dirty_ids_.insert(new_id);
      }
      if (new_id != id) {
        this->dirty_ids_.erase(id);
      }
    }
  }

  /************* Components IDs ****************/
  id::Factory id_factory_;
  std::vector<id_t> components_ids_;
};

} // namespace reveal3d::core
