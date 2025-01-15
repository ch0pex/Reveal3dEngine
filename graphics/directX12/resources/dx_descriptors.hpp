/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_descriptors.hpp
 * @version 1.0
 * @date 14/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <common/common.hpp>
#include <d3d12.h>

namespace reveal3d::graphics::dx12 {

using cpu_handle = D3D12_CPU_DESCRIPTOR_HANDLE;
using gpu_handle = D3D12_GPU_DESCRIPTOR_HANDLE;

struct DescriptorHandle {
  gpu_handle gpu;
  cpu_handle cpu;
  u32 index;
};


} // namespace reveal3d::graphics::dx12
