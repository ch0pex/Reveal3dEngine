/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file script.hpp
 * @version 1.0
 * @date 01/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"

namespace reveal3d::core {

class Entity;

class Script {
public:
    void AssignEntity(Entity &entity) { entity_ = &entity; };
    virtual void Begin() {}
    virtual void Update(f32 dt) { log(logDEBUG) << "Updating"; }
protected:
    Entity *entity_;
};

}

