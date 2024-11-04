/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_debugger.cpp
 * @version 1.0
 * @date 27/02/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_utils.hpp"
#include "dx_checker.hpp"


namespace reveal3d::graphics::dx12::utl {

ID3D12DebugDevice2* reporter;

static void log_adapter_outputs(IDXGIAdapter* adapter);
static void log_output_display_modes(IDXGIOutput* output, DXGI_FORMAT format);

void enable_cpu_layer(reveal3d::u32& factory_flag) {
  ComPtr<ID3D12Debug> debug_controller;
  if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)))) {
    debug_controller->EnableDebugLayer();
    factory_flag |= DXGI_CREATE_FACTORY_DEBUG;
  }
}

void enable_gpu_layer() {
  ComPtr<ID3D12Debug> sp_debug_controller_0;
  ComPtr<ID3D12Debug1> sp_debug_controller_1;
  D3D12GetDebugInterface(IID_PPV_ARGS(&sp_debug_controller_0)) >> DxCheck;
  sp_debug_controller_0->QueryInterface(IID_PPV_ARGS(&sp_debug_controller_1)) >> DxCheck;
  sp_debug_controller_1->SetEnableGPUBasedValidation(TRUE);
}


void log_adapters() {
  u32 index             = 0;
  IDXGIAdapter* adapter = nullptr;
  IDXGIFactory7* factory;
  std::vector<IDXGIAdapter*> adapter_list;

  CreateDXGIFactory(IID_PPV_ARGS(&factory)) >> DxCheck;

  while (factory->EnumAdapters(index, &adapter) != DXGI_ERROR_NOT_FOUND) {
    DXGI_ADAPTER_DESC desc;
    adapter->GetDesc(&desc);

    std::wstring text = L"***Adapter: ";
    text += desc.Description;
    text += L"\n";

    OutputDebugStringW(text.c_str());
    adapter_list.push_back(adapter);
    ++index;
  }
  for (size_t i = 0; i < adapter_list.size(); ++i) {
    log_adapter_outputs(adapter_list[i]);
    adapter_list[i]->Release();
  }
}

void log_adapter_outputs(IDXGIAdapter* adapter) {
  UINT index          = 0;
  IDXGIOutput* output = nullptr;
  while (adapter->EnumOutputs(index, &output) != DXGI_ERROR_NOT_FOUND) {
    DXGI_OUTPUT_DESC desc;
    output->GetDesc(&desc) >> DxCheck;

    std::wstring text = L"***Output: ";
    text += desc.DeviceName;
    text += L"\n";
    OutputDebugStringW(text.c_str());
    ++index;
  }
}

void log_output_display_modes(IDXGIOutput* output, const DXGI_FORMAT format) {
  UINT count       = 0;
  const UINT flags = 0;
  // Call with nullptr to get list countData.
  output->GetDisplayModeList(format, flags, &count, nullptr) >> DxCheck;
  std::vector<DXGI_MODE_DESC> modeList(count);
  output->GetDisplayModeList(format, flags, &count, &modeList[0]) >> DxCheck;
  for (auto& x: modeList) {
    const UINT n      = x.RefreshRate.Numerator;
    const UINT d      = x.RefreshRate.Denominator;
    std::wstring text = L"Width = " + std::to_wstring(x.Width) + L" " + L"Height = " + std::to_wstring(x.Height) +
                        L" " + L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) + L"\n";
    OutputDebugStringW(text.c_str());
  }
}
void queue_info(ID3D12Device* device, const BOOL enable) {
  ComPtr<ID3D12InfoQueue> info_queue;
  device->QueryInterface(IID_PPV_ARGS(&info_queue)) >> DxCheck;
  info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, enable) >> DxCheck;
  //    infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, enable);
  info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, enable) >> DxCheck;
}

void set_reporter(ID3D12Device* device) { device->QueryInterface(&reporter); }

void report_live_device_objs() {
  reporter->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL) >> DxCheck;
  reporter->Release();
}

void get_hardware_adapter(IDXGIFactory1* p_factory, IDXGIAdapter1** pp_adapter) {

  *pp_adapter = nullptr;

  ComPtr<IDXGIAdapter1> adapter;
  ComPtr<IDXGIFactory6> factory6;
  if (SUCCEEDED(p_factory->QueryInterface(IID_PPV_ARGS(&factory6)))) {
    for (u32 adapter_index = 0; SUCCEEDED(factory6->EnumAdapterByGpuPreference(
             adapter_index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)
         ));
         ++adapter_index) {
      DXGI_ADAPTER_DESC1 desc;
      adapter->GetDesc1(&desc) >> DxCheck;

      if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        continue;

      if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
        break;
    }
  }

  if (adapter.Get() == nullptr) {
    for (UINT adapter_index = 0; SUCCEEDED(p_factory->EnumAdapters1(adapter_index, &adapter)); ++adapter_index) {
      DXGI_ADAPTER_DESC1 desc;
      adapter->GetDesc1(&desc) >> DxCheck;

      if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0u)
        continue;
      if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
        break;
    }
  }

  *pp_adapter = adapter.Detach();
}


} // namespace reveal3d::graphics::dx12::utl
