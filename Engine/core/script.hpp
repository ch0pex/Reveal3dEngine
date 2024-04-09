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
//    Script(Entity &entity)  : entity_(entity) {}
    virtual void Begin(Entity &entity) {}
    virtual void Update(Entity &entity, f32 dt) { log(logDEBUG) << "Updating"; }
};

}

