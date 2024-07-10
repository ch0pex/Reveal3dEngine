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

    [[nodiscard]] math::mat4& World() const;
    [[nodiscard]] math::mat4& InvWorld() const;
    [[nodiscard]] math::xvec3 Position() const;
    [[nodiscard]] math::xvec3 Scale() const;
    [[nodiscard]] math::xvec3 Rotation() const;
    [[nodiscard]] math::xvec3 WorldPosition() const;
    [[nodiscard]] math::xvec3 WorldScale() const;
    [[nodiscard]] math::xvec3 WorldRotation() const;


    void SetPosition(math::xvec3 pos) const;
    void SetScale(math::xvec3 size) const;
    void SetRotation(math::xvec3 rot) const;
    void SetWorldPosition(math::xvec3 pos);
    void SetWorldScale(math::xvec3 size);
    void SetWorldRotation(math::xvec3 rot);
    void UpdateWorld();

    INLINE bool IsAlive() const { return id_ != id::invalid; }
    INLINE id_t Id() { return id_; }

    void UnDirty() const;
    void SetDirty() const;
    u8 Dirty() const;
private:
    static math::mat4 CalcWorld(id_t id);
    void UpdateChilds() const;
    id_t id_;
};


}


