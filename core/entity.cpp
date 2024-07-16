/************************************************************************
* Copyright (c) 2024 Alvaro Cabrera Barrio
* This code is licensed under MIT license (see LICENSE.txt for details)
************************************************************************/
/**
* @file entity.hpp
* @version 1.0
* @date 15/07/2024
* @brief Short description
*
* Longer description
*/

#include "entity.hpp"
#include "scene.hpp"

namespace reveal3d::core { 

Entity::Entity(u32 id) : id_ {id} { }

bool Entity::IsAlive() {
    return core::scene.IsEntityAlive(id_);
}


}