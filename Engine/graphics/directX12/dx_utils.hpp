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

#include "dx_common.hpp"
#include <source_location>

namespace reveal3d::graphics::dx12::utl {

template<typename T>
void release(T*& resource) {
    if (resource) {
        resource->Release();
        resource = nullptr;
    }
}

struct Checker {};

extern Checker DxCheck;

struct Error {
    Error(u32 hr, std::source_location = std::source_location::current()) noexcept;
    u32 hr;
    std::source_location loc;
};

void operator>>(Error, Checker);

void EnableCpuLayer(u32 &factoryFlag);
void EnableGpuLayer();
void QueueInfo(ID3D12Device *device, BOOL enable);
void LogAdapters();
void SetReporter(ID3D12Device *device);
void GetHardwareAdapter(IDXGIFactory1 *pFactory, IDXGIAdapter1 **ppAdapter);
void ReportLiveDeviceObjs();
void DisplayFps();

}
