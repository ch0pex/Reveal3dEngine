/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file primitives.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "core/components/geometry.hpp"

namespace reveal3d::content {

template<core::Geometry::Primitive primitive>
core::Geometry::InitInfo import_primitive();

}

