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

void enable_cpu_layer(u32 &factory_flag);
void enable_gpu_layer();
void queue_info(ID3D12Device *device, BOOL enable);
void log_adapters();
void set_reporter(ID3D12Device *device);
void get_hardware_adapter(IDXGIFactory1 *p_factory, IDXGIAdapter1 **pp_adapter);
void report_live_device_objs();
void display_fps();

}
