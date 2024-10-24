/************************************************************************
 * copyright (c) 2024 alvaro cabrera barrio
 * this code is licensed under mit license (see license.txt for details)
 ************************************************************************/
/**
 * @file pool.hpp
 * @version 1.0
 * @date 26/03/2024
 * @brief Pool virtual class
 *
 */

#pragma once

#include "common/common.hpp"
#include "math/math.hpp"

#include <vector>
#include <queue>
#include <set>
#include <type_traits>

namespace reveal3d::core {

namespace detail {

template<typename T>
concept stored_in_gpu = requires(T component) {
    {component.setDirty()} -> std::same_as<void>;
    {component.unDirty()} -> std::same_as<void>;
    {component.dirty()} -> std::same_as<u8>;
};

template<bool InGPU>
struct GpuSynchronize {};

template<>
class GpuSynchronize<true> {
public:
    id_t popNew() {
        if (new_components_.empty()) {
            return id::invalid;
        }
        const auto id = new_components_.front();
        new_components_.pop();
        return id;
    }
    id_t popRemoved() {
        if (deleted_components_.empty()) {
            return id::invalid;
        }
        const auto id = deleted_components_.front();
        deleted_components_.pop();
        return id;
    }
    std::set<id_t>& dirtyElements() { return dirty_ids_; }
    std::set<id_t>& dirtyIds() { return dirty_ids_; }
    utl::vector<u8>& dirties() { return dirties_; }

protected:
    std::queue<id_t>    new_components_;
    std::queue<id_t>    deleted_components_;
    std::set<id_t>      dirty_ids_;
    utl::vector<u8>     dirties_;
};

}

template<typename T>
concept is_component = requires(T component) {
    {component.isAlive()} -> std::same_as<bool>;
    {component.id()} -> std::same_as<id_t>;
    { component.update()} -> std::same_as<void>;
    component.data();
    typename T::InitInfo;
    typename T::Pool;
    typename T::Data;
};

template<is_component T>
class Pool : public detail::GpuSynchronize<detail::stored_in_gpu<T>> {
public:
    T addComponent();
    T addComponent(id_t entity_id);
    T addComponent(id_t entityId, T::InitInfo&& initInfo);
    void removeComponent(id_t entity_id);
    void update();

    u32 count() { return components_data_.count(); }
    T at(id_t id) { return components_ids_.at(id::index(id)); }
    std::vector<T>::iterator begin() { return components_ids_.begin(); };
    std::vector<T>::iterator end() { return components_ids_.end();   };
    u32 getMappedId(id_t component_id) { return id_factory_.mapped(id::index(component_id)); }
    T::Pool& data() { return components_data_; }

private:
    void add(id_t index, id_t id);
    void remove(id_t id);

    /************* Components IDs ****************/
    id::Factory     id_factory_;
    std::vector<T>  components_ids_;

    /************* Component data pool ****************/
    T::Pool         components_data_;
};

template<is_component T>
void Pool<T>::add(id_t index, id_t id) {
    if (index >= components_ids_.size()) {
        components_ids_.emplace_back(id);
    } else {
        components_ids_.at(index) = T(id);
    }
}

template<is_component T>
T Pool<T>::addComponent() {
    components_ids_.emplace_back();
        if constexpr (detail::stored_in_gpu<T>) {
            this->dirties_.emplace_back(4);
        }
        return components_ids_.at(components_ids_.size() - 1);
}

template<is_component T>
void Pool<T>::remove(id_t id) {
    auto last = components_ids_.at(id_factory_.back()).id();
    u32 idx {id_factory_.mapped(id) };
    if (last != id) {
        components_ids_.at(id::index(getMappedId(last))) = T(id::index(id) | id::generation(last));
    }
    components_ids_.at(idx) = {};
    id_factory_.remove(id);
    if constexpr (detail::stored_in_gpu<T>) {
        if (last != id) {
            this->dirties_.at(id::index(id)) = 3;
            this->dirty_ids_.insert(id::index(id) | id::generation(last));
        }
    }
}

template<is_component T>
void Pool<T>::update() {
    if constexpr (detail::stored_in_gpu<T>) {
        for (auto it = this->dirty_ids_.begin(); it != this->dirty_ids_.end();) {
            T component { *it };
            component.update();
            if (this->dirties_.at(id::index(*it)) == 0) {
                it = this->dirty_ids_.erase(it);
            } else {
                ++it;
            }
        }
    } else {
       //  TODO
    }
}


}
