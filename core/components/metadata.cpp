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

Pool<Metadata>& pool = core::scene.componentPool<Metadata>();

Metadata::Metadata(id_t id) : id_(id) { }

std::string& Metadata::name() {
    return pool.data().names.at(id::index(id_));
}

std::string& Metadata::comment() {
    return pool.data().comments.at(id::index(id_));
}

std::string& Metadata::date() {
    return pool.data().dates.at(id::index(id_));
}

void Metadata::update() {

}

Metadata::Data Metadata::data() {
    return { name(), comment(), date() };
}

}
