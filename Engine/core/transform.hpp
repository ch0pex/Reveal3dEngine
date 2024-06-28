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

namespace reveal3d::core {

class Transform {
public:
    struct InitInfo {
        math::xvec3 position;
        math::xvec3 rotation;
        math::xvec3 scale;
    };

    Transform() : id_ { id::invalid }  {}
    explicit Transform(id_t id);
    Transform(id_t id, math::xvec3 pos);
    Transform(id_t id, InitInfo& info);

     math::mat4 World() const;
     math::xvec3 Position() const;
     math::xvec3 Scale() const;
     math::xvec3 Rotation() const;
     u8 IsDirty() const;

    void SetPosition(math::xvec3 pos);
    void SetScale(math::xvec3 size);
    void SetRotation(math::xvec3 rot);
    INLINE void UpdateDirty() { assert(dirty_ > 0); --dirty_; }
private:
    u8 dirty_ { 3 }; // If dirty should update in render buffers
    id_t id_;
};

}


