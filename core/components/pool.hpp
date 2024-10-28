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
#include "concepts.hpp"

#include <vector>
#include <queue>
#include <set>

namespace reveal3d::core {

namespace detail {

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

template<component T>
class Pool : public detail::GpuSynchronize<stored_in_gpu<T>> {
public:
    T addComponent() {
        components_ids_.emplace_back();
        if constexpr (stored_in_gpu<T>) {
            this->dirties_.emplace_back(4);
        }
        return components_ids_.at(components_ids_.size() - 1);
    }

    T addComponent(id_t entity_id) { addComponent(entity_id, {}); }

    T addComponent(id_t entity_id, T::InitInfo&& init_info) {
        id_t component_id{id_factory_.New(id::index(entity_id))};

        data_.add(entity_id, init_info);
        if constexpr(stored_in_gpu<T>) {
            this->dirties().emplace_back(4);
            this->dirtyIds().insert(component_id);
        }

        addId(component_id);

        assert(id::index(component_id) < data_.count());

        return components_ids_.at(id::index(entity_id));

    }
    void removeComponent(id_t entity_id) {
        id_t component_id {components_ids_.at(id::index(entity_id)).id() };
        if (id_factory_.isAlive(component_id)) {
            data_.remove(component_id); // Removes data
            removeId(component_id); // Removes id
        }
    }

    void update() {
        if constexpr (stored_in_gpu<T>) {
            for (auto it = this->dirty_ids_.begin(); it != this->dirty_ids_.end();) {
                T component { *it };
                if constexpr (is_updatable<T>) {
                    component.update();
                }
                if (this->dirties_.at(id::index(*it)) == 0) {
                    it = this->dirty_ids_.erase(it);
                } else {
                    ++it;
                }
            }
        } else if constexpr (is_updatable<T>){
            //  TODO update scripts
            //  TODO update rigid-bodies
        }
    }

    u32 count() { return data_.count(); }

    T at(id_t id) { return components_ids_.at(id::index(id)); }

    std::vector<T>::iterator begin() { return components_ids_.begin(); };

    std::vector<T>::iterator end() { return components_ids_.end();   };

    u32 getMappedId(id_t component_id) { return id_factory_.mapped(id::index(component_id)); }

    T::Pool& data() { return data_; }

private:
    void addId(id_t index, id_t id) {
        if (index >= components_ids_.size()) {
            components_ids_.emplace_back(id);
        } else {
            components_ids_.at(index) = T(id);
        }
    }

    void removeId(id_t id) {
        auto last = components_ids_.at(id_factory_.back()).id();
        u32 idx {id_factory_.mapped(id) };
        if (last != id) {
            components_ids_.at(id::index(getMappedId(last))) = T(id::index(id) | id::generation(last));
        }
        components_ids_.at(idx) = {};
        id_factory_.remove(id);
        if constexpr (stored_in_gpu<T>) {
            if (last != id) {
                this->dirties_.at(id::index(id)) = 3;
                this->dirty_ids_.insert(id::index(id) | id::generation(last));
            }
        }
    }

    /************* Components IDs ****************/
    id::Factory id_factory_;
    std::vector<T> components_ids_;

    /************* Component data pool ****************/
    T::Pool data_;
};

}
