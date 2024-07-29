/************************************************************************
* Copyright (c) 2024 Alvaro Cabrera Barrio
* This code is licensed under MIT license (see LICENSE.txt for details)
************************************************************************/
/**
* @file material.hpp
* @version 1.0
* @date 29/07/2024
* @brief Short description
*
* Longer description
*/

#pragma once

#include "common/common.hpp"
#include "math/math.hpp"

namespace reveal3d::render {

enum Shader : u8 {
    opaque = 0,
    flat, // Not affected by lighting
    grid,
    //    transparent,

    count
};

struct Material {
    math::vec4 baseColor;
};

}

