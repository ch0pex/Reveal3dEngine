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

}

void Script::Update(float dt) {

}

void Script::DisableUpdate() {

}

void Script::EnableUpdate() {

}

void Script::Destroyed() {

}

Script ScriptPool::AddComponent(id_t id) {
    return Script();
}

ScriptPool& Script::Pool() {
    return core::scene.ComponentPool<Script>();
}

Script ScriptPool::AddComponent(id_t id, Script::InitInfo initInfo) {
    return Script();
}

void ScriptPool::RemoveComponent(id_t id) {

}

void ScriptPool::Update() {

}

}
