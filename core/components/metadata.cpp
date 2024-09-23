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
    return Pool().names_.at(id::index(id_));
}

std::string& Metadata::Comment() {
    return Pool().comments_.at(id::index(id_));
}

std::string& Metadata::Date() {
    return Pool().dates_.at(id::index(id_));
}

MetadataPool &Metadata::Pool(){
    return core::scene.ComponentPool<Metadata>();
}

Metadata MetadataPool::AddComponent(id_t id) {
    const id_t metadata_id { id_factory_.New(names_.size()) };

    names_.emplace_back("Entity" + std::to_string(id));
    comments_.emplace_back();
    dates_.emplace_back("10/12/2024");  //TODO
    comments_.at(comments_.size() - 1U).reserve(1024);
    Add(id::index(id), metadata_id);

    return Metadata(metadata_id);
}

Metadata MetadataPool::AddComponent(id_t id, Metadata::InitInfo &&initInfo) {
    const id_t metadata_id { id_factory_.New(names_.size()) };

    names_.emplace_back(std::move(initInfo));
    comments_.emplace_back();
    dates_.emplace_back("10/12/2024");  //TODO
    Add(id::index(id), metadata_id);

    return Metadata(metadata_id);
}

void MetadataPool::RemoveComponent(id_t id) {
    id_t idx { id::index(id) };
    names_.unordered_remove(idx);
    comments_.unordered_remove(idx);
    dates_.unordered_remove(idx);
    id_factory_.Remove(id);
}

void MetadataPool::Update() {
    //Nothing to update for now in metadata
}
}
