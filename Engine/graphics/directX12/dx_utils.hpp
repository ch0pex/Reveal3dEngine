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

namespace reveal3d::graphics::Dx {

struct Checker {};

extern Checker DxCheck;

struct Error {
    Error(u32 hr, std::source_location = std::source_location::current()) noexcept;
    u32 hr;
    std::source_location loc;
};

void operator>>(Error, Checker);

class Debugger {
public:
    inline static void Enable(u32 &factoryFlag) {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
            debugController->EnableDebugLayer();
            factoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }

    static void LogAdapters();
private:
    static void LogAdapterOutputs(IDXGIAdapter* adapter);
    static void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
};


}
