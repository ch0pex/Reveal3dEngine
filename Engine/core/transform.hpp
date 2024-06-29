/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file transform.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "math/math.hpp"
#include "common/id.hpp"
#include <vector>

namespace reveal3d::core {

class Transform {
public:

    Transform() : id_ { id::invalid }  {}
    explicit Transform(id_t id);
    Transform(id_t id, math::mat4& parentWorld);
    Transform(id_t id, math::xvec3 pos);
//    Transform(id_t id, InitInfo& info);

    [[nodiscard]] math::mat4 World() const;
    [[nodiscard]] math::mat4 InvWorld() const;
     math::xvec3 Position() const;
     math::xvec3 Scale() const;
     math::xvec3 Rotation() const;
     void UnDirty() const;
     void SetDirty() const;
     u8 Dirty() const;

    void SetPosition(math::xvec3 pos);
    void SetScale(math::xvec3 size);
    void SetRotation(math::xvec3 rot);
    void UpdateWorld();
private:
    id_t id_;
};


}


