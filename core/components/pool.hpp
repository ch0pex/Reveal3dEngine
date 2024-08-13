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

namespace reveal3d::core {

//NOTE: be careful with performance and inheritance
template<typename T>
class Pool {
public:
    virtual T AddComponent(id_t id) = 0;
    virtual void RemoveComponent(id_t id) = 0;
    virtual void Update() = 0;
    virtual u32  Count() = 0;

    T At(id_t id)                       { return components_.at(id::index(id)); }
    std::vector<T>::iterator begin()    { return components_.begin(); };
    std::vector<T>::iterator end()      { return components_.end();   };


protected:
    void Add(id_t index, id_t id) {
        if (index >= components_.size()) {
            components_.emplace_back(id);
        } else {
            components_.at(index) = T(id);
        }
    }

    void Remove(id_t id) {
        id_t last { id_factory_.Back() };
        id_factory_.Remove(id);
        components_.at(id::index(last)) = T(id::index(id) | id::generation(last));
        components_.at(id::index(id)) = T(id::invalid);
    }

    /************* Components IDs ****************/
    id::Factory                     id_factory_;
    std::vector<T>                  components_;
};

}
