/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_descriptor_heap_type.hpp
 * @version 1.0
 * @date 14/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once
#include <common/primitive_types.hpp>

namespace reveal3d::graphics::dx12 {

enum class HeapType : u8 {
  Cbv     = 0,
  Srv     = 0,
  Uav     = 0,
  Sampler = 1,
  Rtv     = 2,
  Dsv     = 3,
};

}
