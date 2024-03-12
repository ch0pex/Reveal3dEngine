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

#include <format>
#include <stdexcept>
#include <string>
#include <vector>

namespace reveal3d::graphics::dx::utl {

Checker DxCheck;
ID3D12DebugDevice2* reporter;

static void LogAdapterOutputs(IDXGIAdapter* adapter);
static void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

void EnableCpuLayer(reveal3d::u32 &factoryFlag) {
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
        factoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
    }
}

void EnableGpuLayer() {
    ComPtr<ID3D12Debug> spDebugController0;
    ComPtr<ID3D12Debug1> spDebugController1;
    D3D12GetDebugInterface(IID_PPV_ARGS(&spDebugController0)) >> DxCheck;
    spDebugController0->QueryInterface(IID_PPV_ARGS(&spDebugController1)) >> DxCheck;
    spDebugController1->SetEnableGPUBasedValidation(TRUE);
}


void LogAdapters() {
    u32 index = 0;
    IDXGIAdapter *adapter = nullptr;
    IDXGIFactory7 *factory;
    std::vector<IDXGIAdapter *> adapterList;

    CreateDXGIFactory(IID_PPV_ARGS(&factory));

    while (factory->EnumAdapters(index, &adapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        std::wstring text = L"***Adapter: ";
        text += desc.Description;
        text += L"\n";

        OutputDebugStringW(text.c_str());
        adapterList.push_back(adapter);
        ++index;
    }
    for (size_t i = 0; i < adapterList.size(); ++i) {
        LogAdapterOutputs(adapterList[i]);
        adapterList[i]->Release();
    }
}

void LogAdapterOutputs(IDXGIAdapter *adapter) {
    UINT index = 0;
    IDXGIOutput *output = nullptr;
    while (adapter->EnumOutputs(index, &output) != DXGI_ERROR_NOT_FOUND) {
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);

        std::wstring text = L"***Output: ";
        text += desc.DeviceName;
        text += L"\n";
        OutputDebugStringW(text.c_str());
        ++index;
    }
}

void LogOutputDisplayModes(IDXGIOutput *output, DXGI_FORMAT format) {
    UINT count = 0;
    UINT flags = 0;
    // Call with nullptr to get list count.
    output->GetDisplayModeList(format, flags, &count, nullptr);
    std::vector<DXGI_MODE_DESC> modeList(count);
    output->GetDisplayModeList(format, flags, &count, &modeList[0]);
    for (auto &x: modeList) {
        UINT n = x.RefreshRate.Numerator;
        UINT d = x.RefreshRate.Denominator;
        std::wstring text = L"Width = " + std::to_wstring(x.Width) + L" " + L"Height = " + std::to_wstring(x.Height) +
                            L" " + L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) + L"\n";
        OutputDebugStringW(text.c_str());
    }
}
void QueueInfo(ID3D12Device *device, BOOL enable) {
    ComPtr<ID3D12InfoQueue> infoQueue;
    device->QueryInterface(IID_PPV_ARGS(&infoQueue));
    infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, enable);
    infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, enable);
    infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, enable);
}

void SetReporter(ID3D12Device *device) {
    device->QueryInterface(&reporter);
}

void ReportLiveDeviceObjs() {
   reporter-> ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
   reporter->Release();
}

Error::Error(u32 hr, std::source_location location) noexcept : hr(hr), loc(location) {}

void operator>>(Error grabber, Checker checker) {
   if(FAILED(grabber.hr)) {
       std::string error =std::format(
                                   "{}:{}:{}: HRESULT failed with error code {}" ,
                                   grabber.loc.file_name(),
                                   grabber.loc.line(),
                                   grabber.loc.column(),
                                   grabber.hr
                                   );
       throw std::runtime_error(error);
   }
}

}