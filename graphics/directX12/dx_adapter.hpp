/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_adapter.hpp
 * @version 1.0
 * @date 10/11/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "dx_common.hpp"

namespace reveal3d::graphics::dx12 {

struct Adapter {
  Adapter() {
    u32 factory_flags = 0;
#ifdef _DEBUG
    utl::enable_cpu_layer(factory_flags);
    utl::log_adapters();
    utl::enable_gpu_layer();
#endif
    ComPtr<IDXGIAdapter1> hardware_adapter;
    CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&factory)) >> utl::DxCheck;
    utl::get_hardware_adapter(factory.Get(), &hardware_adapter);
    D3D12CreateDevice(hardware_adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)) >> utl::DxCheck;

#ifdef _DEBUG
    utl::queue_info(device.Get(), TRUE);
#endif
  }

  ComPtr<IDXGIFactory5> factory;
  ComPtr<ID3D12Device> device;
};

inline Adapter const adapter;

} // namespace reveal3d::graphics::dx12
