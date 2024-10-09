/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file common.h
 * @version 1.0
 * @date 21/06/2024
 * @brief Short description
 *
 * Generation and index identifiers 
 * 
 *  |----------------------32 bits ---------------------|
 * 
 *  |-----8 bits-----|-------------24 bits--------------|
 * 
 *  -----------------------------------------------------
 *  |  Generation   |              Index                |
 *  -----------------------------------------------------
 *
 *  Invalid Value: 0xFFFFFFFF
 *
 */

#pragma once

#include "primitive_types.hpp"
#include "platform.hpp"
#include "vector.hpp"

#include <typeinfo>
#include <deque>
#include <limits>
#include <vector>

using id_t = reveal3d::u32;

namespace reveal3d::id {

constexpr u32 generationBits    { 8 };
constexpr u32 indexBits         { sizeof(id_t) * 8 - generationBits };
constexpr id_t generationMask   { (id_t { 1 } << generationBits) - 1 };
constexpr id_t indexMask        { (id_t { 1 } << generationBits) - 1 };

constexpr id_t invalid          { ~id_t{ 0 } };
constexpr u32  maxFree          { 1024 };
constexpr u8  maxGeneration     { (std::numeric_limits<u8>::max)() };

using generation_t = std::conditional<generationBits <= 16, std::conditional<generationBits <= 8, u8, u16>, u32>;

constexpr bool isValid(id_t id) {
    return id != invalid;
}

constexpr id_t index(id_t id) {
    return id & indexMask;
}

constexpr id_t generation(id_t id) {
    return (id >> indexBits) & generationMask;
}

constexpr id_t newGeneration(id_t idx) {
    const id_t gen {generation(idx) + 1};
    return index(idx) | (gen << indexBits);
}


class Factory {
public:
    bool UseFree() {
        return (freeIndices_.size() > id::maxFree);
    }

    u32 FreeCount() const{
        return freeIndices_.size();
    }

    bool IsAlive(id_t id) {
        id_t idx {id::index(id)};
        if (idx >= generations_.size()) {
            return false;
        }
        if (id == id::invalid) {
            return false;
        }
        if (id::generation(id) != generations_.at(id::index(id))) {
            return false;
        }
        return true;
    }

    id_t New() {
        id_t id;
        if (UseFree()) {
            id = freeIndices_.front();
            ++generations_[id];
            id |=  (generations_.at(freeIndices_.front()) << indexBits);
            freeIndices_.pop_front();
        } else {
            id = id::index(generations_.size());
            generations_.push_back(0);
            mappedIdx_.push_back(id::index(id));
        }
        owner_ids_.push_back(id);
        mappedIdx_.at(id::index(id)) = id::index(id);
        return id;
    }

    id_t New(u32 index) {
        id_t id;
        if (UseFree()) {
            id = id::index(owner_ids_.size());
            ++generations_[id];
            id |=  (generations_.at(freeIndices_.front()) << indexBits);
            freeIndices_.pop_front();
        } else {
            id = id::index(owner_ids_.size());
            generations_.push_back(0);
            mappedIdx_.push_back(id::index(id));
        }

        owner_ids_.emplace_back(id::index(id));
        mappedIdx_.at(id::index(id)) = index;

        return id;
    }

    void Remove(id_t id) {
        assert(IsAlive(id));
        id_t index { mappedIdx_.at(id::index(id))};
        auto last = Back();

        owner_ids_.unordered_remove(id::index(id));
        mappedIdx_.at(id::index(id)) = index;
        mappedIdx_.at(id::index(last)) = id::invalid;

        if (generations_.at(index) < maxGeneration) {
            freeIndices_.push_back(index);
        }
    }

    id_t Back() {
       return owner_ids_.back();
    }

    id_t Mapped(id_t id) {
       return mappedIdx_.at(id::index(id));
    }


private:
    std::vector<id_t>   generations_;
    std::deque<id_t>    freeIndices_;
    std::vector<id_t>   mappedIdx_; // mappedIdx[componentId] == component index
    utl::vector<id_t>   owner_ids_; // ownerIds[dataIndex] == component id
};

}