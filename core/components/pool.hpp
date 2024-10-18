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

namespace reveal3d::core {

template<typename T>
concept component = requires(T component) {
    {component.IsAlive()} -> std::same_as<bool>;
    {component.Id()} -> std::same_as<id_t>;
};

template<component T>
class Pool {
public:
    T AddComponent();
    T AddComponent(id_t id);
    T AddComponent(id_t entityId, T::InitInfo&& initInfo);
    void RemoveComponent(id_t entity_id);
    void Update();
    u32  Count() { return data_.Count(); }

    T At(id_t id)                       { return components_.at(id::index(id)); }
    std::vector<T>::iterator begin()    { return components_.begin(); };
    std::vector<T>::iterator end()      { return components_.end();   };
    INLINE u32 GetMappedId(id_t componentId) { return id_factory_.Mapped(id::index(componentId)); }
    id_t PopNew();
    id_t PopRemoved();
    INLINE std::set<id_t>&  DirtyElements() { return dirtyIds_; }
    T::Data& Data() { return data_; }

    std::set<id_t>& DirtyIds() { return dirtyIds_; }
    utl::vector<u8>&  Dirties() { return dirties_; }
private:
    void Add(id_t index, id_t id) {
        if (index >= components_.size()) {
            components_.emplace_back(id);
        } else {
            components_.at(index) = T(id);
        }
    }

    void Remove(id_t id) { // TODO This is bugged
        auto last = components_.at(id_factory_.Back()).Id();
        u32 idx { id_factory_.Mapped(id) };
        if (last != id) {
            components_.at(id::index(GetMappedId(last))) = T(id::index(id) | id::generation(last));
        }
        components_.at(idx) = {};
        id_factory_.Remove(id);
    }
    /************* Component Data ****************/
    T::Data data_;

    /************* Components IDs ****************/
    id::Factory       id_factory_;
    std::vector<T>    components_;

    std::queue<id_t>  newComponents_;
    std::queue<id_t>  deletedComponents_;

    std::set<id_t>     dirtyIds_;
    utl::vector<u8>    dirties_;
};

template<component T>
id_t Pool<T>::PopNew() {
    if (newComponents_.empty()) {
        return id::invalid;
    }
    const auto id = newComponents_.front();
    newComponents_.pop();
    return id;
}

template<component T>
id_t Pool<T>::PopRemoved() {
    if (deletedComponents_.empty()) {
        return id::invalid;
    }
    const auto id = deletedComponents_.front();
    deletedComponents_.pop();
    return id;
}

template<component T>
T Pool<T>::AddComponent() {
        components_.emplace_back();
        dirties_.emplace_back(4);
        return components_.at(components_.size() - 1);
}

template<component T>
void Pool<T>::Update() {
    for (auto it = dirtyIds_.begin(); it != dirtyIds_.end();) {
//        id_t idx = id::index(*it);
        T component { *it };
        component.Update();
        if (dirties_.at(id::index(*it)) == 0) {
            it = dirtyIds_.erase(it);
        } else {
            ++it;
        }
    }
//    std::set<id_t> comps_;
//    for (auto comp : components_) {
//       if (comp.Id() == id::invalid)  continue;
//
//        if (comps_.find(comp.Id()) != comps_.end()) {
//            assert(false);
//        }
//        comps_.insert(comp.Id());
//    }

}


}
