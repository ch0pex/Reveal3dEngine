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

#include "platform.hpp"
#include "primitive_types.hpp"
#include "vector.hpp"

#include <deque>
#include <limits>
#include <typeinfo>
#include <vector>

using id_t = reveal3d::u32;

namespace reveal3d::id {

constexpr u32 generationBits{8};
constexpr u32 indexBits{sizeof(id_t) * 8 - generationBits};
constexpr id_t generationMask{(id_t{1} << generationBits) - 1};
constexpr id_t indexMask{(id_t{1} << indexBits) - 1};

constexpr id_t invalid{~id_t{0}};
constexpr u32 maxFree{1024};
constexpr u8 maxGeneration{(std::numeric_limits<u8>::max)()};

using generation_t = std::conditional<generationBits <= 16, std::conditional<generationBits <= 8, u8, u16>, u32>;

constexpr bool is_valid(id_t id) { return id != invalid; }

constexpr id_t index(id_t id) { return id & indexMask; }

constexpr id_t generation(id_t id) { return (id >> indexBits) & generationMask; }

constexpr id_t new_generation(id_t idx) {
    const id_t gen{generation(idx) + 1};
    return index(idx) | (gen << indexBits);
}


class Factory {
public:
    bool useFree() { return (free_indices_.size() > id::maxFree); }

    [[nodiscard]] u32 freeCount() const { return free_indices_.size(); }

    id_t back() { return owner_idx_.back(); }

    id_t mapped(id_t id) { return mapped_idx_.at(id::index(id)); }

    bool isAlive(id_t id) {
        id_t idx{id::index(id)};
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

    id_t newId(u32 index) {
        id_t id;
        if (useFree()) {
            id = free_indices_.front();
            ++generations_[id];
            id |= (generations_.at(free_indices_.front()) << indexBits);
            free_indices_.pop_front();
        }
        else {
            id = id::index(owner_idx_.size());
            generations_.push_back(0);
            mapped_idx_.push_back(id::index(id));
        }

        owner_idx_.emplace_back(index);
        mapped_idx_.at(id::index(id)) = index;

        return id;
    }

    void remove(id_t id) {
        assert(isAlive(id));
        id_t index{mapped_idx_.at(id::index(owner_idx_.size() - 1))};

        mapped_idx_.at(id::index(id)) = index;
        mapped_idx_.at(owner_idx_.size() - 1) = id::invalid;
        owner_idx_.unordered_remove(id::index(id));

        if (generations_.at(id::index(id)) < maxGeneration) {
            free_indices_.push_back(id::index(id));
        }
    }


private:
    std::vector<id_t> generations_;
    std::deque<id_t> free_indices_;
    std::vector<id_t> mapped_idx_; // mappedIdx[componentId] == component index
    utl::vector<id_t> owner_idx_; // ownerIds[dataIndex] == component index
};

} // namespace reveal3d::id
