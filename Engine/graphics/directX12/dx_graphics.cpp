/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file dx_graphics.cpp
 * @version 1.0
 * @date 25/02/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_graphics.hpp"
#include "dx_utils.hpp"

#include "window/window.hpp"
#include <iostream>

namespace reveal3d::graphics::dx {

Graphics::Graphics(const window::Resolution &res) : resolution_(res) {}

void Graphics::LoadPipeline() {
    // Factory -> LookForAdapter -> CreateDevice -> CommandQueue -> SwapChain
    InitDXGIAdapter();
    cmdManager_.Init(device_.Get());
    heapsManager_.rtvHeap.Initialize(device_.Get(), 512, false);
    heapsManager_.dsvHeap.Initialize(device_.Get(), 512, false);
    CreateSwapChain();
    InitFrameResources();
}

// TODO: search for first avaible hardware adapter and look for best performance adapter (GPU)
// TODO: check for features
// TODO: add Info Queue severity
void Graphics::InitDXGIAdapter() {
    u32 factoryFlags = 0;

#ifdef _DEBUG
    utl::EnableCpuLayer(factoryFlags);
    utl::EnableGpuLayer();
#endif
    CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory_)) >> utl::DxCheck;
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_)) >> utl::DxCheck;
#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), TRUE);
#endif
}

void Graphics::CreateSwapChain() {
    ComPtr<IDXGISwapChain1> swapChain1;
    const DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
            .Width = resolution_.width,
            .Height = resolution_.height,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .Stereo = FALSE,
            .SampleDesc = {1, 0},
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = frameBufferCount,
            .Scaling = DXGI_SCALING_STRETCH,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
            .Flags = 0, // TODO: Check feature DXGI_SWWAP_CHAIN_FLAG_ALLOW_TEARING
    };
    factory_->CreateSwapChainForHwnd(cmdManager_.GetQueue(), window_, &swapChainDesc, nullptr, nullptr, &swapChain1) >> utl::DxCheck;
    factory_->MakeWindowAssociation(window_, DXGI_MWA_NO_ALT_ENTER) >> utl::DxCheck; // Disable Alt + Enter for full screen window
    swapChain1.As(&swapChain_) >> utl::DxCheck;
}

void Graphics::InitFrameResources() {
    for(u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        backBuffers_[i].handle = heapsManager_.rtvHeap.alloc();
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i].resource)) >> utl::DxCheck;
        device_->CreateRenderTargetView(backBuffers_[i].resource.Get() ,&desc, backBuffers_[i].handle.cpu);
    }
}

void Graphics::SetViewport() {
    DXGI_SWAP_CHAIN_DESC desc {};

    swapChain_->GetDesc(&desc)  >> utl::DxCheck;
    viewport_.TopLeftX = 0.0f;
    viewport_.TopLeftY = 0.0f;
    viewport_.Height = (f32) desc.BufferDesc.Height;
    viewport_.Width = (f32) desc.BufferDesc.Width;
    viewport_.MaxDepth = 1.0f;
    viewport_.MinDepth = 0.0f;
}

void Graphics::LoadAssets() {}

void Graphics::Update(render::Camera &camera) {}

// TODO: Functions to simplify barrier creation
void Graphics::PopulateCommands() {

    auto &currentBuffer = backBuffers_[Commands::FrameIndex()];
    ID3D12GraphicsCommandList* commandList = cmdManager_.GetList();

    cmdManager_.Reset();

    auto targetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currentBuffer.resource.Get(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET );
    commandList->ResourceBarrier(1, &targetBarrier);



    const f32 clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    commandList->ClearRenderTargetView(currentBuffer.handle.cpu, clearColor, 0, nullptr);

    auto presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currentBuffer.resource.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT
    );
    commandList->ResourceBarrier(1, &presentBarrier);

    commandList->Close() >> utl::DxCheck;
}

void Graphics::Draw() {
    cmdManager_.Execute();
    swapChain_->Present(0, 0) >> utl::DxCheck;
    cmdManager_.MoveToNextFrame();
}


void Graphics::Terminate() {
#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), FALSE);
    utl::SetReporter(device_.Get());
#endif
    cmdManager_.Terminate();
}

}
