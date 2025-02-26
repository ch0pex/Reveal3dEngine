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

#include "resources/dx_deferring_system.hpp"
#include "utils/dx_checker.hpp"
#include "utils/dx_debug.hpp"

namespace reveal3d::graphics::dx12 {

namespace detail {
inline void get_hardware_adapter(IDXGIFactory1* p_factory, IDXGIAdapter1** pp_adapter) {

  *pp_adapter = nullptr;

  ComPtr<IDXGIAdapter1> adapter;
  ComPtr<IDXGIFactory6> factory6;
  if (SUCCEEDED(p_factory->QueryInterface(IID_PPV_ARGS(&factory6)))) {
    for (u32 adapter_index = 0; SUCCEEDED(factory6->EnumAdapterByGpuPreference(
             adapter_index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)
         ));
         ++adapter_index) {
      DXGI_ADAPTER_DESC1 desc;
      adapter->GetDesc1(&desc) >> utils::DxCheck;

      if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        continue;

      if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
        break;
    }
  }

  if (adapter.Get() == nullptr) {
    for (UINT adapter_index = 0; SUCCEEDED(p_factory->EnumAdapters1(adapter_index, &adapter)); ++adapter_index) {
      DXGI_ADAPTER_DESC1 desc;
      adapter->GetDesc1(&desc) >> utils::DxCheck;

      if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0u)
        continue;
      if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
        break;
    }
  }

  *pp_adapter = adapter.Detach();
}

} // namespace detail


struct Adapter {
  Adapter() {
    u32 factory_flags = 0;

#ifdef _DEBUG
    utils::enable_cpu_layer(factory_flags);
    utils::log_adapters();
    utils::enable_gpu_layer();
#endif

    ComPtr<IDXGIAdapter1> hardware_adapter;
    CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&factory)) >> utils::DxCheck;
    detail::get_hardware_adapter(factory.Get(), &hardware_adapter);
    D3D12CreateDevice(hardware_adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)) >> utils::DxCheck;

#ifdef _DEBUG
    utils::queue_info(device.Get(), TRUE);
#endif
  }

  ~Adapter() {
    logger(LogInfo) << "Cleaning deferred resources";
    clean_all_resources();
#ifdef _DEBUG
    utils::queue_info(device.Get(), FALSE);
    utils::set_reporter(device.Get());
#endif
  }

  ComPtr<IDXGIFactory5> factory;
  ComPtr<ID3D12Device> device;
};

inline Adapter const adapter;

} // namespace reveal3d::graphics::dx12
