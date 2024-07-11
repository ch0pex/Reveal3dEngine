/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file light.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"


namespace reveal3d::render {

class Light {
public:
    enum type {
        directional,
        spot,
        cone,

        count
    };

private:
    id_t id_ { id::invalid };
};


}