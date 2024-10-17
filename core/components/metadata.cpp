/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file geometry.cpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */


#include "metadata.hpp"
#include "core/scene.hpp"

namespace reveal3d::core {

Pool<Metadata>& pool() {
    return core::scene.ComponentPool<Metadata>();
}

Metadata::Metadata(id_t id) : id_(id) { }

std::string& Metadata::Name() {
    return pool().Data().names.at(id::index(id_));
}

std::string& Metadata::Comment() {
    return pool().Data().comments.at(id::index(id_));
}

std::string& Metadata::Date() {
    return pool().Data().dates.at(id::index(id_));
}

void Metadata::Update() {

}


}
