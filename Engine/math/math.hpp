/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file math.hpp
 * @version 1.0
 * @date 02/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "vector.hpp"
#include "matrix.hpp"
#include "scalar.hpp"
#include "functions.inl"

namespace reveal3d::math {

const xvec3 direction[6] = {
        {1.0f, 0.0f, 0.0f}, // Forward
        {-1.0f, 0.0f, 0.0f}, // Backward
        {0.0f, 0.0f, 1.0f}, // Up
        {0.0f, 0.0f, -1.0f}, // Down
        {0.0f, -1.0f, 0.0f}, // Left
        {0.0f, 1.0f, 0.0f}, // right
};

}
