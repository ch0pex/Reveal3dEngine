/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_debugger.hpp
 * @version 1.0
 * @date 27/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace reveal3d::graphics::dx12::utils {

void enable_cpu_layer(u32& factory_flag);
void enable_gpu_layer();
void queue_info(ID3D12Device* device, BOOL enable);
void log_adapters();
void set_reporter(ID3D12Device* device);

} // namespace reveal3d::graphics::dx12::utl
