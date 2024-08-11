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

#include <typeinfo>
#include <vector>
#include <deque>

using id_t = reveal3d::u32;

namespace reveal3d::id {

constexpr u32 generationBits { 8 };
constexpr u32 indexBits { sizeof(id_t) * 8 - generationBits };
constexpr id_t generationMask { (id_t { 1 } << generationBits) - 1 };
constexpr id_t indexMask { (id_t { 1 } << generationBits) - 1 };

constexpr id_t invalid { ~id_t{ 0 } };
constexpr u32  maxFree { 1024 };

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
    INLINE bool UseFree() {
        return (freeIndices_.size() > id::maxFree);
    }

    bool IsAlive(id_t id) {
        if (id == id::invalid)
            return false;
        if (id::generation(id) != generations_.at(id::index(id)))
            return false;
        return true;
    }

    id_t New() {
        id_t id { id::invalid };
        if (UseFree()) {
            id = id::newGeneration(freeIndices_.front());
            freeIndices_.pop_front();
            ++generations_[id::index(id)];
        } else {
            id = generations_.size();
            generations_.push_back(0);
        }
        return id;
    }

    void Remove(id_t id) {
        id_t idx { id::index(id) };
        assert(IsAlive(id));
        freeIndices_.push_back(idx);
    }

private:
    std::vector<id_t> generations_;
    std::deque<id_t> freeIndices_;
};

}