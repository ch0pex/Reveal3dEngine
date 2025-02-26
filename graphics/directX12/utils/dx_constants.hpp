/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_constants.hpp
 * @version 1.0
 * @date 05/11/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "d3d12.h"

namespace reveal3d::graphics::dx12 {

constexpr DXGI_FORMAT main_buffer_format  = DXGI_FORMAT_R16G16B16A16_FLOAT;
constexpr DXGI_FORMAT depth_buffer_format = DXGI_FORMAT_D32_FLOAT;

} // namespace reveal3d::graphics::dx12
