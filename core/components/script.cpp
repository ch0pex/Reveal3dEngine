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


Script::Script(id_t id) : id_(id) { }

Script::Script(id_t id, InitInfo script) {

}

void Script::Begin() {
    Pool().Data().scripts.at(id::index(id_))->Begin();
}


void Script::DisableUpdate() {

}

void Script::EnableUpdate() {

}

void Script::Destroyed() {

}


Pool<Script>& Script::Pool() {
    return core::scene.ComponentPool<Script>();
}

void Script::Update() {
    Pool().Data().scripts.at(id::index(id_))->Update(0.3f);
}



}
