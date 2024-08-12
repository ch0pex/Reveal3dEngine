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

namespace reveal3d::core {

//NOTE: be careful with performance ?
template<typename T>
class Pool {
public:
    virtual void RemoveComponent(id_t id) = 0;
    virtual void Update() = 0;
    virtual u32  Count() = 0;

    T At(id_t id)                       { return components_.at(id::index(id)); }
    std::vector<T>::iterator begin()    { return components_.begin(); };
    std::vector<T>::iterator end()      { return components_.end();   };


protected:
    void Remove(id_t id) {
        id_t last { components_.at(id_factory_.Back()) };
        id_factory_.Remove(id);
        last = id::newIndex(last, id::index(id));
        components_.at(id::index(id)) = id::invalid;
    }

    /************* Components IDs ****************/
    id::Factory                     id_factory_;
    std::vector<T>                  components_;
};

}
