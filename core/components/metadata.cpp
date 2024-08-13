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
Metadata MetadataPool::AddComponent(id_t id) {
    id_t id { id_factory_.New() };
    
    if(id_factory_.UseFree()) {
        names_.at(id::index(id)) = "Entity" + std::to_string(id);
        comments_.at(id::index(id)) = "";
        date.at(id::index(id)) = "10/12/2024"; //TODO
    } else {
        names_.emplace_back("Entity" + std::to_string(id));
        comments_.emplace_back(); 
        date.emplace_back("10/12/2024");  //TODO
    }

    return Metadata(id);
}

Transform MetadataPool::AddComponent(id_t id, Metadata::InitInfo &&initInfo) {
    id_t id { id_factory_.New() };

    if(id_factory_.UseFree()) {
        names_.at(id::index(id)) = initInfo;
        comments_.at(id::index(id)) = "";
        date.at(id::index(id)) = "10/12/2024"; //TODO
    } else {
        names_.emplace_back(initInfo);
        comments_.emplace_back(); 
        date.emplace_back("10/12/2024");  //TODO
    }
    return Metadata(id);
}

void MetadataPool::RemoveComponent(id_t id) {
    id_t idx { id::index(id) };
    names_.remove_unordered(idx);
    comments_.remove_unordered(idx);
    dates_.remove_unordered(idx);
    id_factory_.Remove(id);
}

void MetadataPool::Update() {
    //Nothing to update for now in metadata
}

}
