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
 * Longer description
 */

#pragma once

#include "primitive_types.hpp"
#include "platform.hpp"
#include <typeinfo>


namespace reveal3d::id {

using id_t = u32;

constexpr u32 generationBits {8};
constexpr u32 indexBits {sizeof(id_t) * 8 - generationBits};
constexpr id_t generationMask {(id_t {1} << generationBits) - 1};
constexpr id_t indexMask {(id_t {1} << generationBits) - 1};

constexpr id_t invalidId {~id_t{0}};

using generation_t = std::conditional<generationBits <= 16, std::conditional<generationBits <= 8, u8, u16>, u32>;

INLINE bool IsValid(id_t id) {
    return id != invalidId;
}

INLINE id_t Index(id_t id) {
    return id & indexMask;
}

INLINE id_t Generation(id_t id) {
    return (id >> indexBits) & generationMask;
}

INLINE id_t NewGeneration(id_t id) {
    const id_t gen {Generation(id) + 1};
    return Index(id) | (gen << indexBits);
}

}