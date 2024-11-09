/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file rigidbody_pool.hpp
 * @version 1.0
 * @date 31/10/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"

namespace reveal3d::core::rigidbody {

class Pool {
public:
    using init_info = std::string;
    using stored_in_gpu = std::false_type;

private:
};

} // namespace reveal3d::core::rigidbody
