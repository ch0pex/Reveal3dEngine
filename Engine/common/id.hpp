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

using id_t = reveal3d::u32;

namespace reveal3d::id {

constexpr u32 generationBits { 8 };
constexpr u32 indexBits { sizeof(id_t) * 8 - generationBits };
constexpr id_t generationMask { (id_t { 1 } << generationBits) - 1 };
constexpr id_t indexMask { (id_t { 1 } << generationBits) - 1 };

constexpr id_t invalid { ~id_t{ 0 } };
constexpr u32 minFree { 1024 };

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

}