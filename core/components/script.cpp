/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file script.cpp
 * @version 1.0
 * @date 01/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "script.hpp"
#include "core/scene.hpp"

namespace reveal3d::core {

Pool<Script>& pool = core::scene.componentPool<Script>();

Script::Script(id_t id) : id_(id) { }

Script::Script(id_t id, InitInfo script) {

}

void Script::begin() {
    pool.data().scripts.at(id::index(id_))->begin();
}


void Script::disableUpdate() {

}

void Script::enableUpdate() {

}

void Script::destroyed() {

}

void Script::update() {
    pool.data().scripts.at(id::index(id_))->update(0.3f);
}



}
