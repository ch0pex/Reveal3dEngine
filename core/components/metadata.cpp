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

Metadata::Metadata(id_t id) : id_(id) { }

std::string& Metadata::Name() {
    return Pool().Data().names.at(id::index(id_));
}

std::string& Metadata::Comment() {
    return Pool().Data().comments.at(id::index(id_));
}

std::string& Metadata::Date() {
    return Pool().Data().dates.at(id::index(id_));
}

Pool<Metadata>& Metadata::Pool(){
    return core::scene.ComponentPool<Metadata>();
}

void Metadata::Update() {

}


}
